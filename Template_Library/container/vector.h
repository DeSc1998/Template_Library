#pragma once

#include "data_manager.h"

namespace ds {

	template <
		typename Type,
		typename Manager = data_manager<Type> 
	>
	class vector {
	public:
		using value_type = typename Manager::value_type;

		using iterator = typename Manager::iterator;
	private:
		size_t last_elem = 0;
		Manager data;

	public:
		vector() = default;

		vector( const value_type& val ) {
			data[0] = val;
		}

		vector( value_type&& val ) {
			data[0] = std::move(val);
		}


		void push_back( const value_type& val ) {
			data[++last_elem] = val;
		}

		void push_back( value_type&& val ) {
			data[++last_elem] = std::move(val);
		}

		void push_front( const value_type& val ) {
			shift_at();
			data[0] = val;
			++last_elem;
		}

		void push_front( value_type&& val ) {
			shift_at();
			data[0] = std::move(val);
			++last_elem;
		}

		void insert( const value_type& val, size_t index = 0 ) {
			shift_at(index);
			data[index] = val;
			++last_elem;
		}

		void insert(value_type&& val, size_t index = 0) {
			shift_at(index);
			data[index] = std::move(val);
			++last_elem;
		}

		void shift_at( size_t index = 0, long long amount = 1 ) {
			for (size_t i = last_elem; i >= index; i-- )
				data[i + amount] = std::move( data[i] );
		}

		value_type& operator [] ( size_t index ) {
			return data[index];
		}

		const value_type& operator [] (size_t index) const {
			return data[index];
		}

		void erase( size_t index ) {
			shift_at(index, -1);
			--last_elem;
		}

		void erase( const value_type& val ) {
			for (size_t i = 0; i <= last_elem; i++ ) {
				if ( data[i] == val ) {
					shift_at(i, -1);
					--last_elem;
					break;
				}
			}
		}


		iterator begin() {
			return data.begin();
		}

		iterator begin() const {
			return data.begin();
		}

		iterator end() {
			return data.end();
		}

		iterator end() const {
			return data.end();
		}
	};

}