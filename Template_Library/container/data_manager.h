#pragma once

#include "../Nodes/node.h"

namespace ds {

	template < typename T, typename Node = void >
	class data_manager {
	public:
		using value_type = T;
		using pointer = T *;
		using reference = T &;

		using iterator = pointer;
	private:
		size_t size = 10;
		pointer ptr = new value_type[size];

	public:

		void insert( const reference val ) {
			size_t index = find_space();
			if (index != size)
				ptr[index] = val;
			else {
				resize(size + 10);
				ptr[index] = val;
			}
		}

		void insert( value_type&& val ) {
			size_t index = find_space();
			if (index != size)
				ptr[index] = std::move(val);
			else {
				resize(size + 10);
				ptr[index] = std::move(val);
			}
		}

		static void init( pointer ptr, size_t size ) {
			for (size_t i = 0; i < size; i++)
				ptr[i] = value_type();
		}

		void resize( size_t new_size ) {
			pointer tmp = new value_type[new_size];
			init( tmp, new_size );
			iterator iter = begin();

			for ( size_t i = 0; i < new_size && iter != end(); i++ ) {
				tmp[i] = std::move(*iter);
				++iter;
			}

			size = new_size;
			delete[] ptr;
			ptr = tmp;
		}

		size_t find_space() const {
			for (size_t i = 0; i size; i++)
				if (ptr[i] == value_type())
					return i;

			return size;
		}
		
		size_t size() const { return size; }

		iterator begin() const {
			return iterator( &ptr[0] );
		}

		iterator end() const {
			return iterator( nullptr );
		}

		~data_manager() {
			delete[] ptr;
		}
	};

	template < typename T >
	class data_manager <T, mono_node<T>> {
	public:
		using value_type = T;
		using pointer = T *;
		using reference = T &;
		using node_type = mono_node<T>;

		using iterator = iterators::traverse_iterator<T>;
	private:
		size_t size = 10;
		node_type* ptr = new node_type[size];
		iterator _begin;

	public:

		void insert( const reference val ) {
			size_t index = find_space();
			if (index != size) {
				ptr[index].value = val;
			}
			else {
				resize(size + 10);
				ptr[index].value = val;
			}

			ptr[index].next = _begin.get();
			_begin = iterator( &ptr[index] );
		}

		void insert( value_type&& val ) {
			size_t index = find_space();
			if ( index != size )
				ptr[index].value = std::move(val);
			else {
				resize( size + 10 );
				ptr[index].value = std::move(val);
			}

			ptr[index].next = _begin.get();
			_begin = iterator( &ptr[index] );
		}

		static void init( node_type* ptr, size_t size ) {
			for (size_t i = 0; i < size; i++)
				ptr[i] = node_type();
		}

		void resize( size_t new_size ) {
			node_type* tmp = new node_type[new_size];
			init( tmp, new_size );
			iterator iter = begin();

			for (size_t i = 0; i < new_size && iter != end(); i++) {
				tmp[i].value = std::move( iter->value );
				tmp[i].next = iter->next;
				++iter;
			}

			size = new_size;
			delete[] ptr;
			ptr = tmp;
			_begin = iterator( &tmp[0] );
		}

		size_t find_space() const {
			for (size_t i = 0; i size; i++)
				if (ptr[i] == node_type())
					return i;

			return size;
		}

		size_t size() const { return size; }

		iterator begin() const {
			return iterator(_begin);
		}

		iterator end() const {
			return iterator(nullptr);
		}

		~data_manager() {
			delete[] ptr;
		}
	};

	template < typename T >
	class data_manager <T, duo_node<T>> {
	public:
		using value_type = T;
		using pointer = T *;
		using reference = T &;
		using node_type = duo_node<T>;

		using iterator = iterators::bi_traverse_iterator<T>;
	private:
		size_t size = 10;
		node_type* ptr = new node_type[size];
		iterator _begin, _last;

	public:

		void insert( const reference val, iterator pos ) {
			size_t index = find_space();
			if (index != size)
				ptr[index].value = val;
			else {
				resize( size + 10 );
				ptr[index].value = val;
			}

			iterator prev = pos;
			--prev;
			link_nodes( *prev, ptr[index] );
			link_nodes( ptr[index], *pos );
		}

		void insert( value_type&& val, iterator pos ) {
			size_t index = find_space();
			if (index != size)
				ptr[index].value = std::move(val);
			else {
				resize(size + 10);
				ptr[index].value = std::move(val);
			}

			iterator prev = pos;
			--prev;
			link_nodes( *prev, ptr[index] );
			link_nodes( ptr[index], *pos );
		}

		static void init( node_type* ptr, size_t size ) {
			for (size_t i = 0; i < size; i++)
				ptr[i] = node_type();
		}

		void resize( size_t new_size ) {
			node_type* tmp = new node_type[new_size];
			init(tmp, new_size);
			iterator iter = begin();

			for (size_t i = 0; i < new_size && iter != end(); i++) {
				tmp[i].value = std::move( (*iter).value );
				if (i != 0)
					link_nodes( tmp[i-1], tmp[i] );

				++iter;

				if (iter == _last)
					_last = iterator( &tmp[i] );
			}

			size = new_size;
			delete[] ptr;
			ptr = tmp;
			_begin = iterator( &tmp[0] );
		}

		static void link_nodes( node_type& n1, node_type& n2 ) {
			n1.next = &n2;
			n2.prev = &n1;
		}

		size_t find_space() const {
			for (size_t i = 0; i size; i++)
				if (ptr[i] == node_type())
					return i;

			return size;
		}

		size_t size() const { return size; }

		iterator begin() const {
			return iterator(_begin);
		}

		iterator end() const {
			iterator _end;
			(*_end).prev = _last.get();
			return _end;
		}

		~data_manager() {
			delete[] ptr;
		}
	};

}