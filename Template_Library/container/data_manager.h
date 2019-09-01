#pragma once

#include "../Nodes/node.h"

namespace ds {

	template < typename Type, size_t Block_size = 0x800 >
	class block {
	public:
		struct Index { size_t block_index = 0, value_index = 0; };

		using value_type = Type;
		using pointer = Type *;

		static constexpr size_t num_elem = Block_size / sizeof(value_type);
		static constexpr size_t size = num_elem * sizeof(value_type);
		inline static constexpr value_type DEFAULT{0};
	private:
		pointer elems = nullptr;

	public:
		block() = default;

		block( const value_type& val ) : elems( new value_type[num_elem] ) {
			for ( size_t i = 0; i < num_elem; i++ )
				elems[i] = val;
		}

		block( block&& b ) : elems(b.elems) {
			b.elems = nullptr;
		}

		block& operator = ( block&& b ) {
			delete[] elems;
			elems = b.elems;
			b.elems = nullptr;
		}

		block(const block&) = delete;
		block& operator = (const block&) = delete;
		

		static Index parse_index( size_t index ) {
			return Index{ index / num_elem, index % num_elem };
		}

		value_type& operator [] ( size_t index ) {
			return index < num_elem ? elems[index] : const_cast<value_type&>(DEFAULT);
		}

		const value_type& operator [] ( size_t index ) const {
			return index < num_elem ? elems[index] : DEFAULT;
		}


		~block() {
			if ( elems != nullptr )
				delete[] elems;
		}
	};


	template < typename T, typename Node = void >
	class data_manager {
	public:
		using value_type = T;
		using pointer = T *;

		using iterator = pointer;
	private:
		size_t _size = 10;
		pointer elems = new value_type[size];

	public:

		data_manager() : _size(10), elems( new value_type[_size] ) {
			init( elems, _size );
		}

		data_manager( data_manager&& dm ) : _size(dm.size), elems(dm.elems) {
			dm.elems = nullptr;
		}

		data_manager& operator = ( data_manager&& dm ) {
			_size = dm.size;
			delete[] elems;
			elems = dm.elems;
			dm.elems = nullptr;
			return *this;
		}

		data_manager(const data_manager&) = delete;
		data_manager& operator = (const data_manager&) = delete;


		void insert( const value_type& val ) {
			size_t index = find_space();
			if (index != _size)
				elems[index] = val;
			else {
				resize(_size + 10);
				elems[index] = val;
			}
		}

		void insert( value_type&& val ) {
			size_t index = find_space();
			if (index != _size)
				elems[index] = std::move(val);
			else {
				resize(_size + 10);
				elems[index] = std::move(val);
			}
		}

		value_type& operator [] ( size_t index ) {
			if (index < _size)
				return elems[index];
			else
				return ;
		}

		const value_type& operator [] ( size_t index ) const {
			if (index < _size)
				return elems[index];
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
			delete[] elems;
			elems = tmp;
		}

		size_t find_space() const {
			for (size_t i = 0; i < size; i++) {
				if (elems[i] == value_type())
					return i;
			}

			return _size;
		}
		
		size_t size() const { return _size; }

		iterator begin() const {
			return iterator( &elems[0] );
		}

		iterator begin() {
			return iterator(&elems[0]);
		}

		iterator end() const {
			return iterator( ++(&elems[_size-1]) );
		}

		iterator end() {
			return iterator(++(&elems[_size - 1]));
		}

		~data_manager() {
			delete[] elems;
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
		node_type* elems = nullptr;
		iterator _begin;

	public:

		data_manager() : _size(10), elems( new node_type[_size] ) {
			init( elems, _size );
		}

		data_manager( data_manager&& dm ) : elems( dm.elems ) {
			_size = dm._size;
			dm.elems = nullptr;
			_begin = dm._begin;
		}

		data_manager& operator = ( data_manager&& dm ) {
			_size = dm._size;
			delete[] elems;
			elems = dm.elems;
			dm.elems = nullptr;
			_begin = dm._begin;
			return *this;
		}

		data_manager(const data_manager&) = delete;
		data_manager& operator = (const data_manager&) = delete;


		void insert( const value_type& val ) {
			size_t index = find_space();
			if (index != _size) {
				elems[index].value = val;
			}
			else {
				resize(_size + 10);
				elems[index].value = val;
			}

			elems[index].next = _begin.get();
			_begin = iterator( &elems[index] );
		}

		void insert( value_type&& val ) {
			size_t index = find_space();
			if ( index != _size )
				elems[index].value = std::move(val);
			else {
				resize( _size + 10 );
				elems[index].value = std::move(val);
			}

			elems[index].next = _begin.get();
			_begin = iterator( &elems[index] );
		}

		value_type& top() {
			return _begin->value;
		}

		const value_type& top() const {
			return _begin->value;
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
				if ( i != 0 )
					tmp[i-1].next = &tmp[i];
				++iter;
			}

			_size = new_size;
			delete[] elems;
			elems = tmp;
			_begin = iterator( &tmp[0] );
		}

		size_t find_space() const {
			for (size_t i = 0; i < _size; i++) {
				if (elems[i] == node_type())
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
			delete[] elems;
		}
	};

	template < typename T >
	class data_manager <T, duo_node<T>> {
	public:
		using value_type = T;
		using node_type = duo_node<T>;

		using iterator = iterators::bi_traverse_iterator<T>;

		inline static constexpr value_type DEFAULT{ 0 };
	private:
		size_t _size = 10;
		node_type* elems = nullptr;
		iterator _begin;

	public:

		data_manager() : _size(10), elems( new node_type[_size] ), _begin(&elems[0]) {
			init( elems, _size );
		}

		data_manager( data_manager&& dm ) : _size(dm.size), elems(dm.elems) {
			dm.elems = nullptr;
			_begin = dm._begin;
		}

		data_manager& operator = (data_manager&& dm) {
			_size = dm.size;
			delete[] elems;
			elems = dm.elems;
			dm.elems = nullptr;
			_begin = dm._begin;
			return *this;
		}

		data_manager(const data_manager&) = delete;
		data_manager& operator = (const data_manager&) = delete;


		void insert( const value_type& val, size_t index ) {
			if (index >= _size)
				resize( _size + 10 );

			iterator iter = begin();
			for (; index > 0; index-- )
				iter++;

			insert( val, iter );
		}

		void insert( value_type&& val, size_t index ) {
			if (index >= _size)
				resize( _size + 10 );

			iterator iter = begin();
			for (; index > 0; index--)
				iter++;

			insert( std::forward<value_type>(val), iter );
		}

		void insert( const value_type& val, iterator pos ) {
			size_t index = find_space();
			if (index == _size) {
				size_t iter_index = 0;
				while (pos != _begin) { iter_index++; pos--; }
				resize(_size + 10);
				pos = iterator(&elems[iter_index]);
			}

			if ( *_begin != node_type() && pos != _begin ) {
				iterator prev = pos;
				--prev;
				link_nodes(prev, iterator(&elems[index]));
				link_nodes( iterator(&elems[index]), *pos);
			} else if ( *_begin != node_type() && pos == _begin ) {
				link_nodes(elems[index], *pos);
				_begin = iterator(&elems[index]);
				_begin->prev = nullptr;
			}

			elems[index].value = val;
		}

		void insert( value_type&& val, iterator pos ) {
			size_t index = find_space();
			if (index == _size) {
				size_t iter_index = 0;
				while (pos != _begin) { iter_index++; pos--; }
				resize(_size + 10);
				pos = iterator(&elems[iter_index]);
			}

			if ( *_begin != node_type() && pos != _begin ) {
				iterator prev = pos;
				--prev;
				link_nodes( prev, iterator(&elems[index]) );
				link_nodes( iterator(&elems[index]), pos );
			} else if ( pos == _begin && *_begin != node_type() ) {
				link_nodes( elems[index], *pos );
				_begin = iterator(&elems[index]);
				_begin->prev = nullptr;
			}

			elems[index].value = std::move(val);
		}

		value_type& at( size_t index ) {
			iterator iter = _begin;
			for (size_t i = 0; i < index && iter != end(); i++)
				++iter;
			return iter.get() != nullptr ? iter->value : const_cast<value_type&>(DEFAULT);
		}

		const value_type& at( size_t index ) const {
			iterator iter = _begin;
			for (size_t i = 0; i < index && iter != end(); i++)
				++iter;
			return iter.get() != nullptr ? iter->value : DEFAULT;
		}

		bool erase( const value_type& val ) {
			iterator iter = begin();
			for (; iter != end(); iter++ )
				if ( iter->value == val )
					break;

			return erase( iter );
		}

		bool erase( iterator pos ) {
			if ( pos.get() != nullptr ) {
				iterator prev = pos, next = pos;
				--prev; ++next;
				if ( !link_nodes(prev, next) && prev.get() == nullptr )
					_begin = next;
				*pos = node_type();
				return true;
			}
			return false;
		}


		static void check_linkage( node_type* node, size_t size ) {
			for (size_t i = 0; i < size; i++) {
				if ( node[i].next == &node[i] )
					node[i].next = nullptr;

				if ( node[i].prev == &node[i] )
					node[i].prev = nullptr;
			}
		}

		bool is_in_bound( node_type* node ) const {
			return node >= elems && node < elems + _size;
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
				tmp[i].value = std::move( iter->value );
				if (i != 0)
					link_nodes( tmp[i-1], tmp[i] );

				++iter;
			}

			_size = new_size;
			delete[] elems;
			elems = tmp;
			_begin = iterator( &tmp[0] );
		}

		void link_nodes( node_type& n1, node_type& n2 ) {
			n1.next = &n2;
			n2.prev = &n1;
		}

		bool link_nodes( iterator prev, iterator next ) {
			if ( is_in_bound(prev.get()) && is_in_bound(next.get()) ) {
				prev->next = next.get();
				next->prev = prev.get();
				return true;
			}
			return false;
		}

		size_t find_space() const {
			for (size_t i = 0; i < _size; i++) {
				if (elems[i] == node_type())
					return i;
			}

			return _size;
		}

		size_t size() const { return _size; }

		iterator begin() const {
			//check_linkage(elems, _size);
			return iterator(_begin);
		}

		iterator begin() {
			//check_linkage(elems, _size);
			return iterator(_begin);
		}

		iterator end() const {
			return iterator();
		}

		iterator end() {
			return iterator();
		}

		~data_manager() {
			delete[] elems;
		}
	};

}