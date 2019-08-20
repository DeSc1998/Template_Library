
#pragma once

#include "data_manager.h"

namespace ds {

	template < 
		typename Type,
		typename Manager = data_manager< Type, mono_node<Type> > 
	>
	class stack {
	public:
		using value_type = typename Manager::value_type;
		using node_type = typename Manager::node_type;

		using iterator = typename Manager::iterator;

	private:
		size_t num_elements = 0;
		Manager data;

	public:
		stack() = default;

		stack( const value_type& t ) {
			data.insert( t );
			++num_elements;
		}

		stack( value_type&& val ) {
			data.insert( std::forward<value_type>(val) );
			++num_elements;
		}

		stack( stack&& st ) {
			data = std::move(st.data);
			num_elements = st.num_elements;
		}

		stack& operator = ( stack&& st ) {
			data = std::move( st.data );
			num_elements = st.num_elements;
			return *this;
		}

		stack(const stack& st) = delete;
		stack& operator = ( const stack& ) = delete;


		void push( const value_type& t ) {
			data.insert( t );
			++num_elements;
		}

		void push( value_type&& t ) {
			data.insert( std::forward<value_type>(t) );
			++num_elements;
		}

		const value_type& top() const {
			return data.top();
		}

		void pop() {
			data.pop();
			--num_elements;
		}

		bool is_empty() const {
			return num_elements == 0;
		}

		iterator begin() const {
			return data.begin();
		}

		iterator end() const {
			return data.end();
		}

		iterator begin() {
			return data.begin();
		}

		iterator end() {
			return data.end();
		}
	};

}