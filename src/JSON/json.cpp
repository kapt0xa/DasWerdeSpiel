#include "json.h"

#include "json_builder.h"
#include <algorithm>
#include <cmath>

using namespace std;

namespace json
{
	void LogWarningToStream(string_view message, ostream* err_stream_ptr)
	{
		if (err_stream_ptr)
		{
			*err_stream_ptr << message << "\n";
		}
	}

	const std::vector<string> Node::index_to_typename = { "nullptr_t"s, "Array"s, "Dict"s, "string"s, "Int"s, "double"s, "bool"s };

	vector<string>::const_iterator Node::GetType() const
	{
		return index_to_typename.begin() + dynamic_cast<const Node::Value&>(*this).index();
	}

	//vv####################################################################################################################### inside-used functions and objects:

	//vv==================================================================================== print functions and objects:

	//vv----------------------------------------------------- indent handling objects:

	struct PrintContext
	{
		ostream& out;
		int indent_step = 4;
		int indent = 0;
		bool bracket_from_next_line = false;

		void NextLine()
		{
			out.put('\n');
			for (int i = 0; i < indent; ++i)
			{
				out.put(' ');
			}
		}

		void PrepareToBracket()
		{
			if (bracket_from_next_line)
			{
				NextLine();
			}
			bracket_from_next_line = false;
		}

		PrintContext Indented()
		{
			return PrintContext{ out, indent_step, indent + indent_step };
		}
	};

	//vv----------------------------------------------------- print functions:

	class Printer
	{
	public:
		Printer(ostream* err_stream_ptr) :err_stream_ptr_(err_stream_ptr) {}
		Printer() = default;

		void PrintValue(const Node& val, PrintContext& context);
		void PrintValue(nullptr_t, PrintContext& context);
		void PrintValue(const string& text, PrintContext& context);
		void PrintValue(bool b, PrintContext& context);
		void PrintValue(double value, PrintContext& context);
		void PrintValue(Int value, PrintContext& context);
		void PrintValue(const Array& arr, PrintContext& context);
		void PrintValue(const Dict& dict, PrintContext& context);
	private:
		void LogWarning(string_view message)
		{
			LogWarningToStream(message, err_stream_ptr_);
		}

		ostream* err_stream_ptr_ = nullptr;
	};

	void Printer::PrintValue(nullptr_t, PrintContext& context)
	{
		auto& out = context.out;
		out << "null"sv;
	}

	void Printer::PrintValue(const string& text, PrintContext& context)
	{
		auto& out = context.out;
		out << '"';
		for (char c : text)
		{
			switch (c)
			{
			case '\t':
				out << '\t';
				break;
			case '\n':
				out << R"(\n)";
				break;
			case '\r':
				out << R"(\r)";
				break;
			case '\"':
				out << R"(\")";
				break;
			case '\\':
				out << R"(\\)";
				break;
			default:
				out << c;
				break;
			}
		}
		out << '"';
	}

	void Printer::PrintValue(bool b, PrintContext& context)
	{
		auto& out = context.out;
		out << boolalpha << b;
	}

	void Printer::PrintValue(double value, PrintContext& context)
	{
		auto& out = context.out;
		if (!isfinite(value))
		{
			if (isinf(value))
			{
				LogWarning("atempt to print INF"sv);
				if (value > 0)
				{
					out << "1e309"sv;
				}
				else
				{
					out << "-1e309"sv;
				}
				return;
			}
			assert(isnan(value));
			LogWarning("atempt to print NAN"sv);
			PrintValue(nullptr, context);
			return;
		}
		out << value;
	}

	void Printer::PrintValue(Int value, PrintContext& context)
	{
		auto& out = context.out;
		out << value;
	}

	void Printer::PrintValue(const Array& arr, PrintContext& context)
	{
		auto& out = context.out;
		context.PrepareToBracket();
		out << '[';
		auto inside_context = context.Indented();
		bool first_loop = true;
		for (const Node& node : arr)
		{
			if (first_loop)
			{
				first_loop = false;
			}
			else
			{
				out << ',';
			}
			inside_context.NextLine();
			PrintValue(node, inside_context);
		}
		context.NextLine();
		out << ']';
	}

	void Printer::PrintValue(const Dict& dict, PrintContext& context)
	{
		auto& out = context.out;
		context.PrepareToBracket();
		out << '{';
		auto inside_context = context.Indented();
		bool first_loop = true;
		for (const auto& [name, val] : dict)
		{
			if (first_loop)
			{
				first_loop = false;
			}
			else
			{
				out << ',';
			}
			inside_context.NextLine();
			PrintValue(name, inside_context);
			out << ':' << ' ';
			inside_context.bracket_from_next_line = true;
			PrintValue(val, inside_context);
		}
		context.NextLine();
		out << '}';
	}

	void Printer::PrintValue(const Node& val, PrintContext& context)
	{
		visit([&context, this](const auto& value)
			{
				PrintValue(value, context);
			}, dynamic_cast<const Node::variant&>(val));
	}

	//vv====================================================================================== read functions:

	class Loader
	{
	public:
		Loader(ostream* err_stream_ptr) :err_stream_ptr_(err_stream_ptr) {}
		Loader() = default;

		Node LoadNode(istream& input);
		Node LoadNumber(istream& input);
		string LoadString(istream& input);
		Array LoadArray(istream& input);
		Dict LoadDict(istream& input);
	private:
		void LogWarning(string_view message)
		{
			LogWarningToStream(message, err_stream_ptr_);
		}

		ostream* err_stream_ptr_ = nullptr;
	};

	// used for bool/nullptr_t
	void SkipText(istream& input, const string& expected_text)
	{
		const auto end = expected_text.end();
		for (auto it = expected_text.cbegin(); it != end; ++it)
		{
			if (input.get() != *it)
			{
				throw ParsingError("Expected text is \""s +
					expected_text +
					"\", but letter #"s +
					to_string(it - expected_text.cbegin()) +
					" is different"s);
			}
		}
	}

	Node Loader::LoadNumber(istream& input)
	{
		string parsed_num;

		auto read_char = [&parsed_num, &input]
		{
			parsed_num += static_cast<char>(input.get());
			if (!input)
			{
				throw ParsingError("Failed to read number from stream"s);
			}
		};

		auto read_digits = [&input, read_char]
		{
			if (!isdigit(input.peek()))
			{
				throw ParsingError("A digit is expected"s);
			}
			while (isdigit(input.peek()))
			{
				read_char();
			}
		};

		//vv integer part and sign
		if (input.peek() == '-')
		{
			read_char();
		}
		if (input.peek() == '0')
		{
			read_char();
		}
		else
		{
			read_digits();
		}

		bool is_int = true;

		//vv past-point optional part
		if (input.peek() == '.')
		{
			read_char();
			read_digits();
			is_int = false;
		}

		//vv exponentional optional part
		if (int ch = input.peek(); ch == 'e' || ch == 'E')
		{
			read_char();
			if (ch = input.peek(); ch == '+' || ch == '-')
			{
				read_char();
			}
			read_digits();
			is_int = false;
		}

		try
		{
			if (is_int)
			{
				try
				{
					return static_cast<Int>(stoll(parsed_num));
				}
				catch (...)
				{
					LogWarning("Failed to convert "s + parsed_num + " to int, it will become double"s);
				}
			}
			return stod(parsed_num);
		}
		catch (...)
		{
			throw ParsingError("Failed to convert "s + parsed_num + " to number"s);
		}
	}

	string Loader::LoadString(istream& input)
	{
		auto it = istreambuf_iterator<char>(input);
		auto end = istreambuf_iterator<char>();
		string s;

		assert(*it == '"');
		++it;

		while (true)
		{
			if (it == end)
			{
				throw ParsingError("String parsing error, unexpected end of input, closing quotes expected");
			}
			const char ch = *it;
			if (ch == '"')
			{
				++it;
				break;
			}
			else if (ch == '\\')
			{
				++it;
				if (it == end)
				{
					throw ParsingError("String parsing error, unexpected end of input, escape-sequence expected");
				}
				const char escaped_char = *(it);
				switch (escaped_char)
				{
				case 'n':
					s.push_back('\n');
					break;
				case 't':
					s.push_back('\t');
					break;
				case 'r':
					s.push_back('\r');
					break;
				case '"':
					s.push_back('"');
					break;
				case '\\':
					s.push_back('\\');
					break;
				default:
					throw ParsingError("String parsing error, unrecognized escape sequence \\"s + escaped_char);
				}
			}
			else if (ch == '\n' || ch == '\r')
			{
				throw ParsingError("String parsing error, unexpected end of line"s);
			}
			else
			{
				s.push_back(ch);
			}
			++it;
		}

		return s;
	}

	Array Loader::LoadArray(istream& input)
	{
		Array result;
		char c;

		assert(input.peek() == '[');
		input.get();

		input >> c;
		if (c == ']')
		{
			return result;
		}
		else
		{
			input.putback(c);
		}

		while (true)
		{
			if (!input)
			{
				throw ParsingError("Array parsing error, unexpected end of file.");
			}
			result.push_back(LoadNode(input));
			input >> c;
			if (c == ']')
			{
				return result;
			}
			if (c != ',')
			{
				throw ParsingError("Array parsing error, ',' or ']' expected.");
			}
		}
	}

	Dict Loader::LoadDict(istream& input)
	{
		auto ThrowEndOfFile = []()
		{
			throw ParsingError("Dict parsing error, unexpected end of file.");
		};

		Dict result;
		char c;

		assert(input.peek() == '{');
		input.get();

		input >> c;
		if (c == '}')
		{
			return result;
		}
		else
		{
			if (!input)
			{
				ThrowEndOfFile();
			}
			input.putback(c);
		}

		while (true)
		{
			if (!input)
			{
				ThrowEndOfFile();
			}

			pair<string, Node> loaded_pair;
			auto& [name, node] = loaded_pair;

			input >> c;
			if (c != '"')
			{
				throw ParsingError("Dict parsing error, unexpected char, '\"' was expected"s);
			}
			input.putback('"');
			name = LoadString(input);

			input >> c;
			if (c != ':')
			{
				throw ParsingError("Dict parsing error, unexpected char, ':' was expected"s);
			}

			node = LoadNode(input);

			auto [_, is_unique] = result.insert(move(loaded_pair));
			if (!is_unique)
			{
				throw ParsingError("Dict parsing error, name collision"s);
			}

			input >> c;
			if (c == '}')
			{
				return result;
			}
			if (c != ',')
			{
				throw ParsingError("Dict parsing error, ',' or '}' expected.");
			}
		}
	}

	Node Loader::LoadNode(istream& input)
	{
		char c;
		input >> c;
		input.putback(c);

		switch (c)
		{
		case 't':
			SkipText(input, "true"s);
			return Node(true);
			break;
		case 'f':
			SkipText(input, "false"s);
			return Node(false);
			break;
		case 'n':
			SkipText(input, "null"s);
			return Node(nullptr);
			break;
		case '"':
			return Node(LoadString(input));
			break;
		case '[':
			return Node(LoadArray(input));
			break;
		case '{':
			return Node(LoadDict(input));
			break;
		case '-':
			return LoadNumber(input);
			break;
		default:
			if (isdigit(c))
			{
				return LoadNumber(input);
			}
			else
			{
				throw ParsingError("Failed to define node type"s);
			}
			break;
		}
	}

	//vv####################################################################################################################### header-related:

	//vv====================================================================================== class"Node" inplementations

	//vv----------------------- constructors:

	Node::Node(nullptr_t)
		: variant(nullptr)
	{}

	Node::Node(Array array)
		: variant(move(array))
	{}

	Node::Node(Dict map)
		: variant(move(map))
	{}

	Node::Node(Int value)
		: variant(value)
	{}

	Node::Node(double value)
		: variant(value)
	{}

	Node::Node(bool value)
		: variant(value)
	{}

	Node::Node(string value)
		: variant(move(value))
	{}

	//vv-------------------- variable type getters:

	bool Node::IsNull() const
	{
		return holds_alternative<nullptr_t>(*this);
	}

	bool Node::IsBool() const
	{
		return holds_alternative<bool>(*this);
	}

	bool Node::IsInt() const
	{
		return holds_alternative<Int>(*this);
	}

	bool Node::IsDouble() const
	{
		return IsPureDouble() || IsInt();
	}

	bool Node::IsPureDouble() const
	{
		return holds_alternative<double>(*this);
	}

	bool Node::IsString() const
	{
		return holds_alternative<string>(*this);
	}

	bool Node::IsArray() const
	{
		return holds_alternative<Array>(*this);
	}

	bool Node::IsDict() const
	{
		return holds_alternative<Dict>(*this);
	}

	//vv------------------------------------- variable value getters:

	const Array& Node::AsArray() const
	{
		if (IsArray())
		{
			return get<Array>(*this);
		}
		throw logic_error("node doesnt hold array"s);
	}

	Array& Node::AsArray()
	{
		if (IsArray())
		{
			return get<Array>(*this);
		}
		throw logic_error("node doesnt hold array"s);
	}

	const Dict& Node::AsDict() const
	{
		if (IsDict())
		{
			return get<Dict>(*this);
		}
		throw logic_error("node doesnt hold map"s);
	}

	Dict& Node::AsDict()
	{
		if (IsDict())
		{
			return get<Dict>(*this);
		}
		throw logic_error("node doesnt hold map"s);
	}

	Int Node::AsInt() const
	{
		if (IsInt())
		{
			return get<Int>(*this);
		}
		throw logic_error("node doesnt hold int"s);
	}

	double Node::AsDouble() const
	{
		if (IsPureDouble())
		{
			return get<double>(*this);
		}
		if (IsInt())
		{
			return static_cast<double>(get<Int>(*this));
		}
		throw logic_error("node doesnt hold double or int"s);
	}

	bool Node::AsBool() const
	{
		if (IsBool())
		{
			return get<bool>(*this);
		}
		throw logic_error("node doesnt hold bool"s);
	}

	const string& Node::AsString() const
	{
		if (IsString())
		{
			return get<string>(*this);
		}
		throw logic_error("node doesnt hold string"s);
	}

	string& Node::AsString()
	{
		if (IsString())
		{
			return get<string>(*this);
		}
		throw logic_error("node doesnt hold string"s);
	}

	Node::Node(Value&& init)
		:Value(init)
	{}

	//vv--------------------------------- operators

	bool Node::operator==(const Node& other) const
	{
		return static_cast<const variant&>(*this) == static_cast<const variant&>(other);
	}

	bool Node::operator!=(const Node& other) const
	{
		return !(*this == other);
	}

	bool CompareDoubles(double a, double b)
	{
		const double epsilon = 1e-6;
		if (std::abs(b) < 1)
		{
			return std::abs(a - b) <= epsilon;
		}
		else
		{
			return std::abs((a / b) - 1.) <= epsilon;
		}
	}

	Node::Value SmallVersion(const Node& source)
	{
		if (source.IsArray())
		{
			return Builder().StartArray().Value("Array"s).EndArray().Build();
		}
		if (source.IsDict())
		{
			return Builder().StartArray().Value("Dict"s).EndArray().Build();
		}
		if (source.IsString())
		{
			const string& val = get<string>(source);
			constexpr size_t max_size = 32;
			if (val.size() < max_size)
			{
				return val;
			}
			else
			{
				return val.substr(size_t(0), max_size - size_t(3)) + "..."s;
			}
		}
		return source;
	}

	using ProblemReport = optional<Node>;

	Node BuildCompareProblem(const string& description, const Node& l_val, const Node& r_val)
	{
		return Builder().StartDict()
			.Key("prolem").Value(description)
			.Key("left").StartDict()
				.Key("val").Value(SmallVersion(l_val))
				.Key("type").Value(*l_val.GetType())
				.EndDict()
			.Key("right").StartDict()
				.Key("val").Value(SmallVersion(r_val))
				.Key("type").Value(*r_val.GetType())
				.EndDict()
			.EndDict().Build();
	}

	const Node temporrary_report = "<DEFAULT REPORT, THAT SHOULD BE OVERWRITED>"s;

	ProblemReport EqualDebugNode(const Node& l, const Node& r)
	{ 
		bool same_type = l.GetType() == r.GetType();
		if (!same_type)
		{
			return BuildCompareProblem("different types"s, l, r);
		}
		if (l.IsArray())
		{
			ProblemReport problem_report = nullopt;
			Builder report_builder;
			auto problems_storage = report_builder.StartArray().Value("Array");
			const Array& l_arr = get<Array>(l), r_arr = get<Array>(r);
			bool same_size = l_arr.size() == r_arr.size();
			const json::Int size = static_cast<json::Int>(std::min(l_arr.size(), r_arr.size()));
			if (!same_size)
			{
				problems_storage.Value(BuildCompareProblem("different size"s, Int(l_arr.size()), Int(r_arr.size())));
				problem_report = temporrary_report;
			}
			else
			{
				json::Int errors = 0;
				for (json::Int i = 0; i < size; ++i)
				{
					auto problem = EqualDebugNode(l_arr[i], r_arr[i]);
					if (problem)
					{
						problems_storage.StartDict().Key(to_string(i)).Value(*problem);
						if (!problem_report)
						{
							problem_report = temporrary_report;
						}
						++errors;
					}
				}
				problems_storage.StartDict()
					.Key("total errors"s).Value(errors)
					.Key("total elements"s).Value(size);
			}
			if (problem_report)
			{
				problem_report = report_builder.ForceBuild();
			}
			return problem_report;
		}
		else if (l.IsDict())
		{
			ProblemReport problem_report = nullopt;
			Builder report_builder;
			auto problems_storage = report_builder.StartArray().Value("Dict");
			const Dict& l_dict = get<Dict>(l), r_dict = get<Dict>(r);
			bool same_size = l_dict.size() == r_dict.size();
			const json::Int size = static_cast<json::Int>(std::min(l_dict.size(), r_dict.size()));
			if (!same_size)
			{
				problems_storage.Value(BuildCompareProblem("different size"s, Int(l_dict.size()), Int(r_dict.size())));
				problem_report = temporrary_report;
			}
			{
				json::Int errors = 0;
				for (auto& [l_key, l_val] : l_dict)
				{
					auto it = r_dict.find(l_key);
					if (it == r_dict.end())
					{
						problems_storage.Value(BuildCompareProblem("not equal keys"s, l_key, "right key not found"s));
						if (!problem_report)
						{
							problem_report = temporrary_report;
						}
						continue;
					}
					const Node& r_val = it->second;
					auto problem = EqualDebugNode(l_val, r_val);
					if (problem)
					{
						problems_storage.StartDict().Key(l_key).Value(*problem);
						if (!problem_report)
						{
							problem_report = temporrary_report;
						}
						++errors;
					}
				}
				problems_storage.StartDict()
					.Key("total errors"s).Value(errors)
					.Key("total elements"s).Value(size);
			}
			if (problem_report)
			{
				problem_report = report_builder.ForceBuild();
			}
			return problem_report;
		}
		else if (l.IsPureDouble())
		{
			if (!CompareDoubles(get<double>(l), get<double>(r)))
			{
				return BuildCompareProblem("different values"s, l, r);
			}
		}
		else
		{
			if (l != r)
			{
				return BuildCompareProblem("different values"s, l, r);
			}
		}
		return nullopt;
	}

	//vv===================================================================================================== class"Document" and functions related to it

	//vv------------------------------------- class "Document"

	Document::Document(Node root)
		: root_(move(root))
	{}

	const Node& Document::GetRoot() const
	{
		return root_;
	}

	Node& Document::GetRoot()
	{
		return root_;
	}

	bool Document::operator==(const Document& other) const
	{
		return GetRoot() == other.GetRoot();
	}

	bool Document::operator!=(const Document& other) const
	{
		return !(*this == other);
	}

	bool Document::EqualDebug(const Document& other, ostream& log_stream) const
	{
		auto result = EqualDebugNode(GetRoot(), other.GetRoot());
		if (result)
		{
			JSONPrinterLoader().Print(Document(move(*result)), log_stream);
		}
		else
		{
			JSONPrinterLoader().Print(Document("no difference found"s), log_stream);
		}
		return !result;
	}

	//vv------------------------------------------- functions related to "Document"

	Document JSONPrinterLoader::Load(istream& input) const
	{
		return Document{ Loader(err_stream_ptr_).LoadNode(input) };
	}

	Document JSONPrinterLoader::Load(istream&& input) const
	{
		assert(input);
		return Load(input);
	}

	void JSONPrinterLoader::Print(const Document& doc, ostream& output, int indent_step, int start_indent) const
	{
		PrintContext context = { output, indent_step, start_indent };
		Printer(err_stream_ptr_).PrintValue(doc.GetRoot(), context);
	}

	void JSONPrinterLoader::Print(const Document& doc, ostream&& output, int indent_step, int start_indent) const
	{
		assert(output);
		Print(doc, output, indent_step, start_indent);
	}

	JSONPrinterLoader& JSONPrinterLoader::SetLogErrStream(ostream& err_stream)
	{
		err_stream_ptr_ = &err_stream;
		return *this;
	}
}