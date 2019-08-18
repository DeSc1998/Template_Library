
#pragma once

#include <type_traits>

#include "mono_node.h"
#include "duo_node.h"

namespace ds {

	template < typename T >
	class is_node : public std::false_type {};

	template < typename T >
	class is_node < mono_node<T> > : public std::true_type {};

	template < typename T >
	class is_node < duo_node<T> > : public std::true_type {};

	template < typename T >
	constexpr bool is_node_v = is_node<T>::value;


	template < typename T >
	class is_mono_node : public std::false_type {};

	template < typename T >
	class is_mono_node < mono_node<T> > : public is_node< mono_node<T> > {};

	template < typename T >
	constexpr bool is_mono_node_v = is_mono_node<T>::value;


	template < typename T >
	class is_duo_node : public std::false_type {};

	template < typename T >
	class is_duo_node < duo_node<T> > : public is_node< duo_node<T> > {};

	template < typename T >
	constexpr bool is_duo_node_v = is_duo_node<T>::value;


	namespace iterators {

		// iterator base for node based containers
		template <
			typename Type,
			typename Node_Type,
			typename = std::enable_if_t< is_node_v<Node_Type> >
		>
			class iterator_base {
			public:
				using value_type = Type;
				using node_type = Node_Type;

			protected:
				node_type* ptr = nullptr;

			public:
				iterator_base() = default;

				iterator_base(node_type* ptr) {
					this->ptr = ptr;
				}

				iterator_base(const iterator_base& iter) {
					ptr = iter.ptr;
				}

				iterator_base(iterator_base&& iter) {
					ptr = iter.ptr;
					iter.ptr = nullptr;
				}


				value_type& operator * () {
					return ptr->value;
				}

				const value_type& operator * () const {
					return ptr->value;
				}

				node_type* operator -> () {
					return ptr;
				}

				const node_type* operator -> () const {
					return ptr;
				}

				node_type* get() {
					return ptr;
				}

				const node_type* get() const {
					return ptr;
				}


				bool operator == ( const iterator_base& iter ) const {
					return ptr == iter.ptr;
				}

				bool operator != ( const iterator_base& iter ) const {
					return !( *this == iter );
				}


				virtual iterator_base& operator ++ () = 0;
				virtual iterator_base& operator ++ (int) = 0;
				virtual iterator_base& operator -- () { return *this; }
				virtual iterator_base& operator -- (int) { return *this; }
		};

		template < typename Type >
		using mono_iter_base = iterator_base< Type, mono_node<Type> >;

		template < typename Type >
		using duo_iter_base = iterator_base< Type, duo_node<Type> >;


		template < typename Type >
		class traverse_iterator : public mono_iter_base<Type> {
			using iterator_base_type = mono_iter_base<Type>;

			using node_type = typename iterator_base_type::node_type;
		public:

			traverse_iterator() : iterator_base_type() {}

			traverse_iterator( node_type* ptr ) : iterator_base_type( ptr ) {}

			traverse_iterator( const traverse_iterator& iter ) :
				iterator_base_type( reinterpret_cast<const iterator_base_type&>(iter) ) {}

			traverse_iterator( traverse_iterator&& iter ) : 
				iterator_base_type( reinterpret_cast<iterator_base_type&&>(iter) ) {}


			traverse_iterator& operator = ( const traverse_iterator& iter ) {
				this->ptr = iter.ptr;
				return *this;
			}


			iterator_base_type& operator ++ () override {
				this->ptr = this->ptr->next;
				return *this;
			}

			iterator_base_type& operator ++ (int) override {
				this->ptr = this->ptr->next;
				return *this;
			}
		};


		template < typename Type >
		class bi_traverse_iterator : public duo_iter_base<Type> {
			using iterator_base_type = duo_iter_base<Type>;

			using node_type = typename iterator_base_type::node_type;
		public:

			bi_traverse_iterator() : iterator_base_type() {}

			bi_traverse_iterator( node_type* ptr ) : iterator_base_type(ptr) {}

			bi_traverse_iterator( const bi_traverse_iterator& iter ) :
				iterator_base_type( reinterpret_cast<const iterator_base_type&>(iter) ) {}

			bi_traverse_iterator( bi_traverse_iterator&& iter ) :
				iterator_base_type( reinterpret_cast<iterator_base_type&&>(iter) ) {}


			bi_traverse_iterator& operator = (const bi_traverse_iterator& iter) {
				this->ptr = iter.ptr;
				return *this;
			}


			iterator_base_type& operator ++ () {
				this->ptr = this->ptr->next;
				return *this;
			}

			iterator_base_type& operator ++ (int) {
				this->ptr = this->ptr->next;
				return *this;
			}

			iterator_base_type& operator -- () {
				this->ptr = this->ptr->prev;
				return *this;
			}

			iterator_base_type& operator -- (int) {
				this->ptr = this->ptr->prev;
				return *this;
			}
		};

	}

}
