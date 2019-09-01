

#pragma once

#include "../Types.h"

namespace ds {

	template < typename Type >
	class duo_node {
	public:
		Type value{};
		duo_node *prev = nullptr, *next = nullptr;


		duo_node() = default;

		duo_node( const Type& t ) : value(t) {}

		duo_node( const Type& t, const_pointer<duo_node> _next, const_pointer<duo_node> _prev = nullptr ) :
			value( t ), next(_next), prev(_prev)
		{}

		duo_node( const duo_node& node ) : 
			value(node.value), next(node.next), prev(node.prev)
		{}

		duo_node( duo_node&& node ) {
			value = std::move( node.value );
			next = node.next;
			node.next = nullptr;
			prev = node.prev;
			prev = nullptr;
		}


		duo_node& operator = ( const duo_node& node ) {
			value = node.value;
			next = node.next;
			prev = node.prev;
			return *this;
		}

		duo_node& operator = ( duo_node&& node ) {
			value = std::move(node.value);
			next = node.next;
			node.next = nullptr;
			prev = node.prev;
			prev = nullptr;
			return *this;
		}


		bool operator== ( const duo_node& node ) const {
			return ( value == node.value && next == node.next && prev == node.prev );
		}

		bool operator!= ( const duo_node& node ) const {
			return !( *this == node );
		}
	};

}