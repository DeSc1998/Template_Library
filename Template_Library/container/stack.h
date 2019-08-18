
#pragma once

#include <memory>

#include "../Nodes/node.h"
#include "data_manager.h"

namespace ds {

	template < typename Type, typename Manager = data_manager<Type, mono_node<Type>> >
	class stack {
	public:
		using node_type = mono_node<Type>;

		const node_type Default = node_type{0u};

		using iterator = iterators::traverse_iterator<Type>;

	private:
		size_t current_size = 10, num_elements = 0;
		node_type* head = nullptr;
		std::unique_ptr<node_type[]> elements = std::make_unique<node_type[]>( current_size );

	public:
		stack() = default;

		stack( const Type& t ) {
			elements[0].value = t;
			head = &elements[0];
			++num_elements;
		}

		stack( const stack& st ) {

		}

		stack( stack&& st ) {
			head = st.head;
			st.head = nullptr;
			elements = std::move( st.elements );
			current_size = st.current_size;
			num_elements = st.num_elements;
		}

		stack& operator= ( const stack& ) = delete;

		void push( const Type& t ) {

			if ( num_elements < current_size ) {

				// finde freien Platz
				for ( unsigned int i = 1; i < this->current_size; i++ ) {
					if ( elements[i] == node_type() ) {
						elements[i] = node_type(t, head);
						head = &elements[i];

						break;
					}
				}

			}
			else {

				// Speicherzuweisung
				auto temp_alloc = std::make_unique<node_type[]>( current_size + 10 );

				for ( unsigned int i = 0; i < current_size; i++ ) {
					temp_alloc[i] = elements[i];
				}

				elements = std::move( temp_alloc );
				current_size += 10;

				auto index = current_size - 10;

				elements[index].value = t;
				elements[index].next = head;
				head = &elements[index];
			}

			++num_elements;
		}

		const Type& top() const {
			if ( head != nullptr )
				return head->value;
			else
				return Type{};
		}

		void pop() {
			if ( head != nullptr ) {
				node_type* new_head = head->next;
				*head = node_type();
				head = new_head;
				--num_elements;
			}
		}

		bool is_empty() const {
			return num_elements == 0;
		}

		iterator begin() const {
			return iterator( head );
		}

		iterator end() const {
			return iterator();
		}
	};

}