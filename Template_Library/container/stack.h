
#pragma once

#include "data_manager.h"

namespace ds {

	template < 
		typename T,
		typename Manager = data_manager< T > 
	>
	class stack {
	public:
		using value_type = typename Manager::value_type;

		using iterator = typename Manager::reverse_iterator;

	private:
		size_t num_elements = 0;
		Manager data;

	public:
		stack() = default;

		stack( const value_type& t ) noexcept {
			data[num_elements++] = t;
		}

		stack( value_type&& val ) noexcept {
			data[num_elements++] = std::move( val );
		}

		stack( const stack& st ) :
			data( st.data ),
			num_elements( st.num_elements )
		{}

		stack( stack&& st ) noexcept {
			data = std::move(st.data);
			num_elements = st.num_elements;
		}

		stack& operator = ( stack&& st ) noexcept {
			data = std::move( st.data );
			num_elements = st.num_elements;
			return *this;
		}

		stack& operator = ( const stack& st ) {
			data = st.data;
			num_elements = st.num_elements;

			return *this;
		}


		void push( const value_type& t ) {
			data[num_elements++] = t;
		}

		void push( value_type&& t ) {
			data[num_elements++] = std::move( t );
		}

		value_type& top() noexcept {
			return data[num_elements - 1];
		}

		const value_type& top() const noexcept {
			return data[num_elements - 1];
		}

		void pop() noexcept {
			if ( num_elements != 0 )
				data[--num_elements] = value_type();
		}

		size_t size() noexcept {
			return num_elements;
		}

		bool is_empty() const noexcept {
			return num_elements == 0;
		}

		iterator end() const noexcept {
			return data.begin();
		}

		iterator begin() const noexcept {
			return data.iterator_at( num_elements );
		}

		iterator end() noexcept {
			return data.begin();
		}

		iterator begin() noexcept {
			return data.iterator_at( num_elements );
		}
	};

}