
#pragma once

#include "../Types.h"

namespace ds {

	template < typename T >
	class mono_node {
	public:
		T value{};
		mono_node* next = nullptr;


		mono_node() = default;

		mono_node( const T& t ) : value(t) {}

		mono_node( const T& t, const_pointer<mono_node> n ) : value(t), next(n) {}

		mono_node( const mono_node& node ) {
			value = node.value;
			next = node.next;
		}

		mono_node( mono_node&& node ) {
			value = std::move( node.value );
			next = node.next;
			node.next = nullptr;
		}

		mono_node& operator = ( const mono_node& node ) {
			value = node.value;
			next = node.next;
			return *this;
		}

		mono_node& operator = ( mono_node&& node ) {
			value = std::move( node.value );
			next = node.next;
			node.next = nullptr;
			return *this;
		}


		bool operator== (const mono_node& other) const {
			return (this->value == other.value && this->next == other.next);
		}

		bool operator!= (const mono_node& other) const {
			return !(*this == other);
		}
	};

}