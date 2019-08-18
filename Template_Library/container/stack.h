
#pragma once

#include "data_manager.h"

namespace ds {

	template < 
		typename Type,
		typename Manager = data_manager< Type, mono_node<Type> > 
	>
	class stack {
	public:
		//using node_type = mono_node<Type>;
		using node_type = typename Manager::node_type;

		//using iterator = iterators::traverse_iterator<Type>;
		using iterator = typename Manager::iterator;

	private:
		size_t num_elements = 0;
		Manager data;

		// OLD IMPLEMENTATION
		//size_t current_size = 10;
		//node_type* head = nullptr;
		//std::unique_ptr<node_type[]> elements = std::make_unique<node_type[]>( current_size );

	public:
		stack() = default;

		stack( const Type& t ) {
			// OLD IMPLEMENTATION
			//elements[0].value = t;
			//head = &elements[0];
			data.insert( t );
			++num_elements;
		}

		stack( stack&& st ) {
			// OLD IMPLEMENTATION
			//head = st.head;
			//st.head = nullptr;
			//elements = std::move( st.elements );
			//current_size = st.current_size;
			data = std::move(st.data);
			num_elements = st.num_elements;
		}

		stack(const stack& st) = delete;
		stack& operator = ( const stack& ) = delete;


		void push( const Type& t ) {
			// OLD IMPLEMENTATION
			//if ( num_elements < current_size ) {
			//
			//	// finde freien Platz
			//	for ( unsigned int i = 1; i < this->current_size; i++ ) {
			//		if ( elements[i] == node_type() ) {
			//			elements[i] = node_type(t, head);
			//			head = &elements[i];
			//
			//			break;
			//		}
			//	}
			//
			//}
			//else {
			//
			//	// Speicherzuweisung
			//	auto temp_alloc = std::make_unique<node_type[]>( current_size + 10 );
			//
			//	for ( unsigned int i = 0; i < current_size; i++ ) {
			//		temp_alloc[i] = elements[i];
			//	}
			//
			//	elements = std::move( temp_alloc );
			//	current_size += 10;
			//
			//	auto index = current_size - 10;
			//
			//	elements[index].value = t;
			//	elements[index].next = head;
			//	head = &elements[index];
			//}

			data.insert( t );

			++num_elements;
		}

		const Type& top() const {
			// OLD IMPLEMENTATION
			//if ( head != nullptr )
			//	return head->value;
			//else
			//	return Type{};
			return data.top();
		}

		void pop() {
			// OLD IMPLEMENTATION
			//if ( head != nullptr ) {
			//	node_type* new_head = head->next;
			//	*head = node_type();
			//	head = new_head;
			//	--num_elements;
			//}
			data.pop();
			--num_elements;
		}

		bool is_empty() const {
			return num_elements == 0;
		}

		iterator begin() const {
			// OLD IMPLEMENTATION
			//return iterator( head );
			return data.begin();
		}

		iterator end() const {
			// OLD IMPLEMENTATION
			//return iterator();
			return data.end();
		}

		iterator begin() {
			// OLD IMPLEMENTATION
			//return iterator( head );
			return data.begin();
		}

		iterator end() {
			// OLD IMPLEMENTATION
			//return iterator();
			return data.end();
		}
	};

}