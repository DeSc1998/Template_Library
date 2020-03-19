#pragma once

#include "../Types.h"

namespace ds {
	template < typename T >
	class duo_node {
	public:
		T value{};
		duo_node* prev = nullptr, * next = nullptr;

		duo_node() = default;

		duo_node(const T& t) : value(t) {}

		duo_node(const T& t, const_pointer<duo_node> _next, const_pointer<duo_node> _prev = nullptr) :
			value(t), prev(_prev), next(_next)
		{}

		duo_node(const duo_node& node) :
			value(node.value), prev(node.prev), next(node.next)
		{}

		duo_node(duo_node&& node) {
			value = std::move(node.value);
			next = node.next;
			node.next = nullptr;
			prev = node.prev;
			prev = nullptr;
		}

		duo_node& operator = (const duo_node& node) {
			value = node.value;
			next = node.next;
			prev = node.prev;
			return *this;
		}

		duo_node& operator = (duo_node&& node) {
			value = std::move(node.value);
			next = node.next;
			node.next = nullptr;
			prev = node.prev;
			prev = nullptr;
			return *this;
		}

		bool operator== (const duo_node& node) const {
			return (value == node.value && next == node.next && prev == node.prev);
		}

		bool operator!= (const duo_node& node) const {
			return !(*this == node);
		}
	};
}