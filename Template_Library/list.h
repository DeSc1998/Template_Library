
#pragma once

#include "Nodes/node.h"

#include <iterator>

namespace ds {

	template < typename Type >
	class list {
	public:

		using node_type = duo_node<Type>;
		
		using iterator = iterators::bi_traverse_iterator<Type>;

	private:

		size_t num_elements = 0, size = 10;
		std::unique_ptr<node_type[]> elements = std::make_unique<node_type[]>( size );
		iterator _begin;
		

		static void init( std::unique_ptr<node_type[]>& ptr, size_t size, node_type _default = node_type() ) {
			for ( size_t i = 0; i < size; i++ ) {
				ptr[i] = _default;
			}
		}

		static void link_nodes( node_type& first, node_type& second ) {
			first.next = &second;
			second.prev = &first;
		}

	public:
		list() {
			init( elements, size );
		}

		list( const Type& t ) {
			init( elements, size );
			elements[0].value = t;
			_begin = iterator( &elements[0] );
			num_elements++;
		}


		// Listenoperatotionen
		void insert( const Type& t ) {
			bool inserted = false;
			for ( size_t i = 0; i < size; i++ ) {
				if ( elements[i] == node_type() ) {
					elements[i].value = t;
					inserted = true;
					link_nodes( *end(), elements[i] );
					break;
				}
			}

			if ( !inserted ) {
				resize( size + 10 );
				elements[size - 10].value = t;
				link_nodes( *_end, elements[size - 10] );
				++_end;
			}

			num_elements++;
		}

		void insert( const Type& t, size_t index ) {
			auto iter = begin();

			while (index > 0) {
				++iter;
				--index;
			}

			insert( t, iter );
		}

		void insert( const Type& t, iterator iter ) {
			bool inserted = false;
			iterator iter_2 = iter;
			--iter_2;
			node_type* n = &elements[0];

			for ( size_t i = 0; i < size; i++ ) {
				if (*n == node_type()) {
					n->value = t;
					inserted = true;
					break;
				}
				++n;
			}
			
			if ( !inserted ) {
				resize( size + 10 );
				n = &elements[0];
				n += size - 10;
				n->value = t;
			}

			// [ ..., iter_2, n, iter, ... ]
			link_nodes( *iter_2, *n );
			link_nodes( *n, *iter );

			++num_elements;
		}

		void insert( Type&& t ) {
			bool inserted = false;
			for (size_t i = 0; i < size; i++) {
				if (elements[i] == node_type()) {
					elements[i].value = std::move(t);
					inserted = true;
					link_nodes(*end(), elements[i]);
					break;
				}
			}

			if (!inserted) {
				resize(size + 10);
				elements[size - 10].value = std::move(t);
				link_nodes(*_end, elements[size - 10]);
				++_end;
			}

			num_elements++;
		}

		void insert( Type&& t, size_t index ) {
			auto iter = begin();

			while (index > 0) {
				++iter;
				--index;
			}

			insert(std::forward<Type>(t), iter);
		}

		void insert( Type&& t, iterator iter ) {
			bool inserted = false;
			iterator iter_2 = iter;
			--iter_2;
			node_type* n = &elements[0];

			for (size_t i = 0; i < size; i++) {
				if (*n == node_type()) {
					n->value = std::move(t);
					inserted = true;
					break;
				}
				++n;
			}

			if (!inserted) {
				resize(size + 10);
				n = &elements[0];
				n += size - 10;
				n->value = std::move(t);
			}

			// [ ..., iter_2, n, iter, ... ]
			link_nodes(*iter_2, *n);
			link_nodes(*n, *iter);

			++num_elements;
		}

		void resize( size_t new_size ) {
			std::unique_ptr<node_type[]> temp = std::make_unique<node_type[]>(new_size);
			auto iter = begin();

			init(temp, new_size);

			for (size_t i = 0; i < new_size && iter != end(); i++) {
				temp[i] = *iter;
				++iter;
				
				if (i != 0)
					link_nodes( temp[i-1], temp[i] );
			}

			_begin = iterator( &temp[0] );

			size = new_size;
			ptr = std::move( temp );
		}

		bool is_empty() const {
			return num_elements == 0;
		}


		// Iteratoren
		iterator begin() const {
			return _begin;
		}

		iterator end() const {
			return iterator();
		}
	};


}