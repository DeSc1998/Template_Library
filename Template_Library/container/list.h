
#pragma once

#include <iostream>

#include "../Nodes/node.h"
#include "data_manager.h"

namespace ds {

	template < 
		typename T,
		typename Manager = data_manager< T, duo_node<T> >
	>
	class list {
	public:

		using value_type = typename Manager::value_type;
		using node_type = typename Manager::node_type;

		using iterator = typename Manager::iterator;

	private:

		size_t num_elements = 0;
		Manager data;

	public:
		list() = default;

		list(const value_type& t) {
			data.insert(t);
			num_elements++;
		}


		// Listenoperatotionen
		void insert(const value_type& t, size_t index = 0) {
			data.insert(t, index);
			++num_elements;
		}

		void insert(const value_type& t, iterator iter) {
			data.insert(t, iter);

			++num_elements;
		}

		void insert(value_type&& t, size_t index = 0) {
			data.insert(std::forward<value_type>(t), index);
			++num_elements;
		}

		void insert(value_type&& val, iterator iter) {
			data.insert(std::forward<value_type>(val), iter);

			++num_elements;
		}

		value_type& operator [] (size_t index) {
			return data.at(index);
		}

		const value_type& operator [] (size_t index) const {
			return data.at(index);
		}

		void erase(const value_type& val) {
			if ( data.erase(val) )
				--num_elements;
		}

		void erase(iterator pos) {
			if ( data.erase(pos) )
				--num_elements;
		}

		void resize(size_t new_size) {
			data.resize(new_size);
		}

		bool is_empty() const {
			return num_elements == 0;
		}

		size_t size() const {
			return num_elements;
		}


		// Iteratoren
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