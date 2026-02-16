#pragma once

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <unordered_map>
#include <deque>
#include <variant>
#include <cassert>
#include <optional>

namespace json
{
    using std::string;
    using std::istream;
    using std::ostream;
    using std::nullptr_t;
    using std::variant;
    using std::ptrdiff_t;
    using std::optional;
    using std::nullopt;

    using Int = ptrdiff_t;

    class Node;
    using Dict = std::map<string, Node>;
    using Array = std::vector<Node>;

    class ParsingError : public std::runtime_error {
    public:
        using runtime_error::runtime_error;
    };

    class Node final : public variant<nullptr_t, Array, Dict, string, Int, double, bool>
    {
    public:

        using variant = variant;
        using Value = variant;

        Node(Value&& init);

        Node() = default;
        Node(nullptr_t);
        Node(Array array);
        Node(Dict map);
        Node(Int value);
        Node(double value);
        Node(bool value);
        Node(string value);

        bool IsNull() const;
        bool IsBool() const;
        bool IsInt() const;
        bool IsDouble() const;
        bool IsPureDouble() const;
        bool IsString() const;
        bool IsArray() const;
        bool IsDict() const;

        const Array& AsArray() const;
        const Dict& AsDict() const;
        Int AsInt() const;
        double AsDouble() const;
        bool AsBool() const;
        const string& AsString() const;

        Array& AsArray();
        Dict& AsDict();
        string& AsString();

        bool operator == (const Node& other) const;
        bool operator != (const Node& other) const;

        std::vector<string>::const_iterator GetType() const;
    private:
        friend class Builder;

        static const std::vector<string> index_to_typename;
    };

    class Document {
    public:
        explicit Document(Node root);

        const Node& GetRoot() const;
        Node& GetRoot();

        bool operator == (const Document& other) const;
        bool operator != (const Document& other) const;

        bool EqualDebug(const Document& other, ostream& log_stream) const; // like operator == but with logs

    private:
        Node root_;
    };

    class JSONPrinterLoader
    {
    public:
        Document Load(istream& input) const;
        Document Load(istream&& input) const;
        void Print(const Document& doc, ostream& output, int indent_step = 2, int start_indent = 0) const;
        void Print(const Document& doc, ostream&& output, int indent_step = 2, int start_indent = 0) const;
        JSONPrinterLoader& SetLogErrStream(ostream& err_stream);
    private:
        ostream* err_stream_ptr_ = nullptr;
    };

}  // namespace json