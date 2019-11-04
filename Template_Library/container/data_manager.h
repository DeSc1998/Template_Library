#pragma once

#include "block.h"
#include "../Nodes/node.h"

namespace ds {

	template < typename T, typename Node = void >
	class data_manager {
	public:
		using block_type = block<T>;

		using value_type      = typename block_type::value_type;
		using reference       = typename block_type::reference;
		using const_reference = typename block_type::const_reference;
		using pointer         = typename block_type::pointer;

		using iterator = block_iterator<value_type, block_type::block_size()>;
	private:
		size_t _block_count = 10, _size = 0;
		std::unique_ptr<block_type[]> elems = std::make_unique<block_type[]>( _block_count );

	public:

		data_manager() {
			elems[0] = block_type( value_type() );
			++_size;
		}

		data_manager( data_manager&& dm ) : 
			_block_count(dm._block_count),
			_size(dm._size),
			elems( std::move(dm.elems) ) 
		{}

		data_manager(const data_manager&) = delete;

		data_manager& operator = ( data_manager&& dm ) {
			_block_count = dm._block_count;
			elems = std::move(dm.elems);
			return *this;
		}

		data_manager& operator = (const data_manager&) = delete;


		void insert( const value_type& val ) {
			auto iter = find_space();
			if ( iter != end() )
				*iter = val;
			else if (_size < _block_count) {
				elems[_size++] = block_type( value_type() );
				++(--iter);
				*iter = std::move(val);
			} else {
				resize(_block_count + 10);
				elems[_size++] = block_type( value_type() );
				++(--iter);
				*iter = val;
			}
		}

		void insert( value_type&& val ) {
			auto iter = find_space();
			if ( iter != end() )
				*iter = std::move(val);
			else if (_size < _block_count) {
				++(--iter);
				*iter = std::move(val);
			} else {
				resize(_block_count + 10);
				++(--iter);
				*iter = std::move(val);
			}
		}

		value_type& operator [] ( size_t index ) noexcept {
			if ( index >= _size * block_type::num_elem )
				expand_by(1);

			const auto block_index = index / block_type::num_elem;
			const auto value_index = index % block_type::num_elem;
			return elems[block_index][value_index];
		}

		const value_type& operator [] ( size_t index ) const noexcept {
			if (index >= _size * block_type::num_elem)
				expand_by(1);

			const auto block_index = index / block_type::num_elem;
			const auto value_index = index % block_type::num_elem;
			return elems[block_index][value_index];
		}


		void resize( size_t new_block_count ) {
			std::unique_ptr<block_type[]> tmp =
				std::make_unique<block_type[]>(new_block_count);

			for ( size_t i = 0; i < new_block_count && i < _block_count; i++ )
				tmp[i] = std::move( elems[i] );

			_block_count = new_block_count;
			elems = std::move(tmp);
			expand_by(1);
		}

		void expand_by( size_t blocks ) {
			if (_size == _block_count)
				resize( _block_count + blocks + 1 );

			for (; blocks > 0; --blocks) {
				elems[_size++] = block_type( value_type() );
			}
		}

		iterator find_space() const {
			auto iter = begin();
			auto last = end();

			for (; iter != last; ++iter) {
				if (*iter == value_type())
					return iter;
			}

			return last;
		}
		
		constexpr size_t size() const noexcept { return end() - begin(); }

		constexpr iterator begin() const {
			return iterator( elems[0].begin(), std::addressof(elems[0]) );
		}

		constexpr iterator begin() {
			return iterator( elems[0].begin(), std::addressof(elems[0]) );
		}

		constexpr iterator iterator_at( size_t index ) {
			return begin() + index;
		}

		constexpr iterator iterator_at( size_t index ) const {
			return begin() + index;
		}

		constexpr iterator end() const {
			return iterator( elems[_size - 1].end(), std::addressof(elems[_size - 1]) );
		}

		constexpr iterator end() {
			return iterator( elems[_size - 1].end(), std::addressof(elems[_size - 1]) );
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
			_begin = iterator( std::addressof(elems[index]) );
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
			_begin = iterator( std::addressof(elems[index]) );
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
					tmp[i-1].next = std::addressof(tmp[i]);
				++iter;
			}

			_size = new_size;
			delete[] elems;
			elems = tmp;
			_begin = iterator( std::addressof(tmp[0]) );
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
				pos = iterator( std::addressof(elems[iter_index]) );
			}

			if ( *_begin != node_type() && pos != _begin ) {
				iterator prev = pos;
				--prev;
				link_nodes(prev, iterator( std::addressof(elems[index]) ));
				link_nodes( iterator( std::addressof(elems[index]) ), *pos);
			} else if ( *_begin != node_type() && pos == _begin ) {
				link_nodes(elems[index], *pos);
				_begin = iterator( std::addressof(elems[index]) );
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
				pos = iterator( std::addressof(elems[iter_index]) );
			}

			if ( *_begin != node_type() && pos != _begin ) {
				iterator prev = pos;
				--prev;
				link_nodes( prev, iterator( std::addressof(elems[index]) ) );
				link_nodes( iterator( std::addressof(elems[index]) ), pos );
			} else if ( pos == _begin && *_begin != node_type() ) {
				link_nodes( elems[index], *pos );
				_begin = iterator( std::addressof(elems[index]) );
				_begin->prev = nullptr;
			}

			elems[index].value = std::move(val);
		}

		value_type& at( size_t index ) {
			iterator iter = _begin;
			for (size_t i = 0; i < index && iter != end(); i++)
				++iter;
			assert(iter.get() != nullptr);
			return iter->value;
		}

		const value_type& at( size_t index ) const {
			iterator iter = _begin;
			for (size_t i = 0; i < index && iter != end(); i++)
				++iter;
			assert(iter.get() != nullptr);
			return iter->value;
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
				if ( node[i].next == std::addressof(node[i]) )
					node[i].next = nullptr;

				if ( node[i].prev == std::addressof(node[i]) )
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
			_begin = iterator( std::addressof(tmp[0]) );
		}

		void link_nodes( node_type& n1, node_type& n2 ) {
			n1.next = std::addressof(n2);
			n2.prev = std::addressof(n1);
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