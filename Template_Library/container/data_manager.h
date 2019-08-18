#pragma once

#include "../Nodes/node.h"

namespace ds {

	template < typename T, typename Node = void >
	class data_manager {
	public:
		using value_type = T;
		using pointer = T *;

		using iterator = pointer;
	private:
		size_t _size = 10;
		pointer ptr = new value_type[size];

	public:

		data_manager() : _size(10), ptr( new value_type[_size] ) {
			init( ptr, _size );
		}

		data_manager( data_manager&& dm ) : _size(dm.size), ptr(dm.ptr) {
			dm.ptr = nullptr;
		}

		data_manager& operator = ( data_manager&& dm ) {
			_size = dm.size;
			delete[] ptr;
			ptr = dm.ptr;
			dm.ptr = nullptr;
			return *this;
		}

		data_manager(const data_manager&) = delete;
		data_manager& operator = (const data_manager&) = delete;


		void insert( const value_type& val ) {
			size_t index = find_space();
			if (index != _size)
				ptr[index] = val;
			else {
				resize(_size + 10);
				ptr[index] = val;
			}
		}

		void insert( value_type&& val ) {
			size_t index = find_space();
			if (index != _size)
				ptr[index] = std::move(val);
			else {
				resize(_size + 10);
				ptr[index] = std::move(val);
			}
		}

		value_type& operator [] ( size_t index ) {
			if (index < _size)
				return ptr[index];
			else
				return ;
		}

		const value_type& operator [] ( size_t index ) const {
			if (index < _size)
				return ptr[index];
			else
				return ;
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

			_size = new_size;
			delete[] ptr;
			ptr = tmp;
		}

		size_t find_space() const {
			for (size_t i = 0; i < size; i++) {
				if (ptr[i] == value_type())
					return i;
			}

			return _size;
		}
		
		size_t size() const { return _size; }

		iterator begin() const {
			return iterator( &ptr[0] );
		}

		iterator begin() {
			return iterator(&ptr[0]);
		}

		iterator end() const {
			return iterator( ++(&ptr[_size-1]) );
		}

		iterator end() {
			return iterator(++(&ptr[_size - 1]));
		}

		~data_manager() {
			delete[] ptr;
		}
	};

	template < typename T >
	class data_manager <T, mono_node<T>> {
	public:
		using value_type = T;
		using node_type = mono_node<T>;

		using iterator = iterators::traverse_iterator<T>;
	private:
		size_t _size = 10;
		node_type* ptr = nullptr;
		iterator _begin;

	public:

		data_manager() : _size(10), ptr( new node_type[_size] ) {
			init( ptr, _size );
		}

		data_manager( data_manager&& dm ) : ptr( dm.ptr ) {
			_size = dm._size;
			dm.ptr = nullptr;
			_begin = dm._begin;
		}

		data_manager& operator = ( data_manager&& dm ) {
			_size = dm._size;
			delete[] ptr;
			ptr = dm.ptr;
			dm.ptr = nullptr;
			_begin = dm._begin;
			return *this;
		}

		data_manager(const data_manager&) = delete;
		data_manager& operator = (const data_manager&) = delete;


		void insert( const value_type& val ) {
			size_t index = find_space();
			if (index != _size) {
				ptr[index].value = val;
			}
			else {
				resize(_size + 10);
				ptr[index].value = val;
			}

			ptr[index].next = _begin.get();
			_begin = iterator( &ptr[index] );
		}

		void insert( value_type&& val ) {
			size_t index = find_space();
			if ( index != _size )
				ptr[index].value = std::move(val);
			else {
				resize( _size + 10 );
				ptr[index].value = std::move(val);
			}

			ptr[index].next = _begin.get();
			_begin = iterator( &ptr[index] );
		}

		value_type& top() {
			return *_begin;
		}

		const value_type& top() const {
			return *_begin;
		}

		void pop() {
			auto next = _begin->next;
			*_begin = node_type();
			_begin = iterator( next );
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

			_size = new_size;
			delete[] ptr;
			ptr = tmp;
			_begin = iterator( &tmp[0] );
		}

		size_t find_space() const {
			for (size_t i = 0; i < _size; i++) {
				if (ptr[i] == node_type())
					return i;
			}

			return _size;
		}

		size_t size() const { return _size; }

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
		using node_type = duo_node<T>;

		using iterator = iterators::bi_traverse_iterator<T>;
	private:
		size_t _size = 10;
		node_type* ptr = nullptr;
		iterator _begin;

	public:

		data_manager() : _size(10), ptr( new node_type[_size] ) {
			init( ptr, _size );
		}

		data_manager( data_manager&& dm ) : _size(dm.size), ptr(dm.ptr) {
			dm.ptr = nullptr;
			_begin = dm._begin;
		}

		data_manager& operator = (data_manager&& dm) {
			_size = dm.size;
			delete[] ptr;
			ptr = dm.ptr;
			dm.ptr = nullptr;
			_begin = dm._begin;
			return *this;
		}

		data_manager(const data_manager&) = delete;
		data_manager& operator = (const data_manager&) = delete;


		void insert( const value_type& val, iterator pos ) {
			size_t index = find_space();
			if (index != _size)
				ptr[index].value = val;
			else {
				resize( _size + 10 );
				ptr[index].value = val;
			}

			iterator prev = pos;
			--prev;
			link_nodes( *prev, ptr[index] );
			link_nodes( ptr[index], *pos );
		}

		void insert( value_type&& val, iterator pos ) {
			size_t index = find_space();
			if (index != _size)
				ptr[index].value = std::move(val);
			else {
				resize(_size + 10);
				ptr[index].value = std::move(val);
			}

			iterator prev = pos;
			--prev;
			link_nodes( *prev, ptr[index] );
			link_nodes( ptr[index], *pos );
		}

		value_type& at( size_t index ) {
			iterator iter = _begin;
			for (size_t i = 0; i <= index && iter != end(); i++)
				++iter;
			return *(--iter);
		}

		const value_type& at( size_t index ) const {
			iterator iter = _begin;
			for (size_t i = 0; i <= index && iter != end(); i++)
				++iter;
			return *(--iter);
		}

		void erase( const value_type& val ) {
			iterator iter = begin();
			for (; iter != end(); iter++ )
				if ( (*iter).value == val )
					break;
			
			iterator prev = iter, next = iter;
			--prev; ++next;
			link_nodes( *prev, *next );
			*iter = node_type();
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
			}

			_size = new_size;
			delete[] ptr;
			ptr = tmp;
			_begin = iterator( &tmp[0] );
		}

		static void link_nodes( node_type& n1, node_type& n2 ) {
			n1.next = &n2;
			n2.prev = &n1;
		}

		size_t find_space() const {
			for (size_t i = 0; i < size; i++) {
				if (ptr[i] == node_type())
					return i;
			}

			return _size;
		}

		size_t size() const { return _size; }

		iterator begin() const {
			return iterator(_begin);
		}

		iterator begin() {
			return iterator(_begin);
		}

		iterator end() const {
			return iterator();
		}

		iterator end() {
			return iterator();
		}

		~data_manager() {
			delete[] ptr;
		}
	};

}