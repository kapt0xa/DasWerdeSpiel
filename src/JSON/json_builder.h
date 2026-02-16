#pragma once
#include "json.h"
#include <vector>
#include <stdexcept>
#include <optional>
#include <cassert>
#include <stdexcept>

namespace json
{
	using std::string;
	using std::vector;
	using std::optional;
	using std::nullopt;
	using std::is_same_v;
	using std::move;
	using std::decay_t;
	using std::get;
	using std::logic_error;

	class Builder;

	namespace basic_builders
	{
		template<typename Parent>
		class OpenNode;
		template<typename Parent>
		class OpenArray;
		template<typename Parent>
		class OpenDict;

		class Complete
		{
		private:
			Node result_;

		public:
			using Root = Complete;
			using InitT = OpenNode<Complete>;

			Node Build();

		private:
			friend Builder;
			InitT BuildInit();
		};

		template<typename Parent>
		class OpenNode
		{
		private:
			Node& context_node_;
			Parent& parent_;
		public:
			using Root = typename Parent::Root;
			using SiblingArray = OpenArray<Parent>;
			using SiblingDict = OpenDict<Parent>;
			static_assert(is_same_v<Root, Complete>);


			Parent& Value(const char* val);
			Parent& Value(Node::Value val = nullptr);
			SiblingArray& StartArray();
			SiblingDict& StartDict();

		private:
			OpenNode(Parent& parent, Node& context_node);
			friend Parent;

			operator SiblingArray& ();
			operator SiblingDict& ();
		};

		template<typename Parent>
		class OpenArray
		{
		private:
			Node& context_node_;
			Parent& parent_;

			Array& GetArray();
		public:
			using Root = typename Parent::Root;
			using ChildArray = OpenArray<OpenArray>;
			using ChildDict = OpenDict<OpenArray>;
			static_assert(is_same_v<Root, Complete>);

			OpenArray& Value(const char* val);
			OpenArray& Value(Node::Value val);
			ChildArray StartArray();
			ChildDict StartDict();
			Parent& EndArray();

			OpenArray& Reserve(size_t capacity);
			OpenArray& ShrinkToFit();

		private:
			OpenArray(Parent& parent, Node& context_node);
			friend Parent;
		};

		template<typename Parent>
		class OpenDict
		{
		private:
			Node& context_node_;
			Parent& parent_;

			Dict& GetDict();
		public:
			using Root = typename Parent::Root;
			using ChildNode = OpenNode<OpenDict>;
			static_assert(is_same_v<Root, Complete>);

			ChildNode Key(string key);
			Parent& EndDict();

			OpenDict& Reserve(size_t capacity);
			OpenDict& Rehash(size_t buskets);

		private:
			OpenDict(Parent& parent, Node& context_node);
			friend Parent;
		};
	}

	class Builder
	{
	private:
		basic_builders::Complete root_;
		basic_builders::OpenNode<basic_builders::Complete> body_ = root_.BuildInit();

		void Reset();
	public:
		using InitT = basic_builders::OpenNode<basic_builders::Complete>;
		using ValInitT = InitT::Root;
		using ArrInitT = InitT::SiblingArray;
		using DictInitT = InitT::SiblingDict;

		// this method resets the Builder
		ValInitT Value(const char* val);
		// this method resets the Builder
		ValInitT Value(Node::Value val = nullptr);
		// this method resets the Builder
		ArrInitT StartArray();
		// this method resets the Builder
		DictInitT StartDict();

		//makes invalid all child objects, that are returned by Value(...), StartArray() or StartDict()
		Node ForceBuild();
	};
	
	//===================================================================================================== template implementations

	namespace basic_builders
	{
		template<typename Parent>
		inline Parent& OpenNode<Parent>::Value(Node::Value val)
		{
			assert(context_node_.IsNull());
			dynamic_cast<Node::Value&>(context_node_) = val;
			return parent_;
		}

		template<typename Parent>
		inline Parent& OpenNode<Parent>::Value(const char* val)
		{
			return Value(Node::Value(string(val)));
		}

		template<typename Parent>
		inline typename OpenNode<Parent>::SiblingArray& OpenNode<Parent>::StartArray()
		{
			assert(context_node_.IsNull());
			context_node_ = Array();
			return *this;
		}

		template<typename Parent>
		inline typename OpenNode<Parent>::SiblingDict& OpenNode<Parent>::StartDict()
		{
			assert(context_node_.IsNull());
			context_node_ = Dict();
			return *this;
		}

		template<typename Parent>
		inline OpenNode<Parent>::OpenNode(Parent& parent, Node& context_node)
			: context_node_(context_node), parent_(parent)
		{
			assert(context_node_.IsNull());
		}

		template<typename Parent>
		inline OpenNode<Parent>::operator SiblingArray& ()
		{
			return reinterpret_cast<SiblingArray&>(*this);
		}

		template<typename Parent>
		inline OpenNode<Parent>::operator SiblingDict& ()
		{
			return reinterpret_cast<SiblingDict&>(*this);
		}

		template<typename Parent>
		inline Array& OpenArray<Parent>::GetArray()
		{
			return get<Array>(dynamic_cast<Node::Value&>(context_node_));
		}

		template<typename Parent>
		inline OpenArray<Parent>& OpenArray<Parent>::Value(Node::Value val)
		{
			GetArray().push_back(Node(move(val)));
			return *this;
		}

		template<typename Parent>
		inline OpenArray<Parent>& OpenArray<Parent>::Value(const char* val)
		{
			return Value(Node::Value(string(val)));
		}

		template<typename Parent>
		inline typename OpenArray<Parent>::ChildArray OpenArray<Parent>::StartArray()
		{
			return ChildArray(*this, GetArray().emplace_back(Node(Array())));
		}

		template<typename Parent>
		inline typename OpenArray<Parent>::ChildDict OpenArray<Parent>::StartDict()
		{
			return ChildDict(*this, GetArray().emplace_back(Node(Dict())));
		}

		template<typename Parent>
		inline Parent& OpenArray<Parent>::EndArray()
		{
			return parent_;
		}

		const bool is_array_vector = is_same_v<Array, std::vector<Node>>;

		template<typename Parent>
		inline OpenArray<Parent>& OpenArray<Parent>::Reserve(size_t capacity)
		{
			if constexpr (is_array_vector)
			{
				GetArray().reserve(capacity);
			}
			return *this;
		}

		template<typename Parent>
		inline OpenArray<Parent>& OpenArray<Parent>::ShrinkToFit()
		{
			if constexpr (is_array_vector)
			{
				GetArray().shrink_to_fit();
			}
			return *this;
		}

		template<typename Parent>
		inline OpenArray<Parent>::OpenArray(Parent& parent, Node& context_node)
			: context_node_(context_node), parent_(parent)
		{}

		template<typename Parent>
		inline Dict& OpenDict<Parent>::GetDict()
		{
			return get<Dict>(dynamic_cast<Node::Value&>(context_node_));
		}

		template<typename Parent>
		inline typename OpenDict<Parent>::ChildNode OpenDict<Parent>::Key(string key)
		{
			auto [it, no_collision] = GetDict().try_emplace(move(key));
			auto& [emplaced_key, emplaced_node] = *it;
			if (!no_collision)
			{
				throw logic_error("key collision: " + emplaced_key);
			}
			return ChildNode(*this, emplaced_node);
		}

		template<typename Parent>
		inline Parent& OpenDict<Parent>::EndDict()
		{
			return parent_;
		}

		const bool is_dict_unordered = is_same_v<Dict, std::unordered_map<string, Node>>;

		template<typename Parent>
		inline OpenDict<Parent>& OpenDict<Parent>::Reserve(size_t buskets)
		{
			if constexpr (is_dict_unordered)
			{
				GetDict().reserve(buskets);
			}
			return *this;
		}

		template<typename Parent>
		inline OpenDict<Parent>& OpenDict<Parent>::Rehash(size_t buskets)
		{
			if constexpr (is_dict_unordered)
			{
				GetDict().rehash(buskets);
			}
			return *this;
		}

		template<typename Parent>
		inline OpenDict<Parent>::OpenDict(Parent& parent, Node& context_node)
			: context_node_(context_node), parent_(parent)
		{}
	}
}
