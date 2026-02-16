#include "json_builder.h"
#include <cassert>

namespace json
{
	using namespace std::string_literals;
	using std::logic_error;
	using std::holds_alternative;
	
	namespace basic_builders
	{
		Node Complete::Build()
		{
			return move(result_);
		}

		typename Complete::InitT Complete::BuildInit()
		{
			return InitT(*this, result_);
		}
	}

	void Builder::Reset()
	{
		root_.result_ = Node();
	}

	Builder::ValInitT Builder::Value(Node::Value val)
	{
		Reset();
		return body_.Value(move(val));
	}

	Builder::ValInitT Builder::Value(const char* val)
	{
		return Value(Node::Value(string(val)));
	}

	Builder::ArrInitT Builder::StartArray()
	{
		Reset();
		return body_.StartArray();
	}

	Builder::DictInitT Builder::StartDict()
	{
		Reset();
		return body_.StartDict();
	}

	Node Builder::ForceBuild()
	{
		return root_.Build();
	}
}
