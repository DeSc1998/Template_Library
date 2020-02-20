#pragma once

#include <cassert>

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

		using iterator         = block_iterator<value_type, block_type::block_size()>;
		using reverse_iterator = reverse_block_iterator<value_type, block_type::block_size()>;
	private:
		size_t Block_count = 10, Size = 0;
		std::unique_ptr<block_type[]> elems = std::make_unique<block_type[]>( Block_count );

	public:

		data_manager() {
			elems[Size++] = block_type( value_type() );
		}

		data_manager( const data_manager& other ) :
			elems( other.elems ),
			Block_count( other.Block_count ),
			Size( other.Size )
		{}

		data_manager( data_manager&& dm ) : 
			elems( std::move(dm.elems) ), 
			Block_count( dm.Block_count ),
			Size( dm.Size )
		{}

		data_manager& operator = ( const data_manager& dm ) {
			elems = dm.elems;
			Block_count = dm.Block_count;
			Size = dm.Size;

			return *this;
		}

		data_manager& operator = ( data_manager&& dm ) {
			elems = std::move(dm.elems);
			Block_count = dm.Block_count;
			Size = dm.Size;

			return *this;
		}

		[[deprecated]]
		void insert( const value_type& val ) {
			auto iter = find_space();
			if ( iter != end() )
				*iter = val;
			else if (Size < Block_count) {
				elems[Size++] = block_type( value_type() );
				++(--iter);
				*iter = std::move(val);
			} else {
				resize(Block_count + 10);
				elems[Size++] = block_type( value_type() );
				++(--iter);
				*iter = val;
			}
		}

		[[deprecated]]
		void insert( value_type&& val ) {
			auto iter = find_space();
			if ( iter != end() )
				*iter = std::move(val);
			else if (Size < Block_count) {
				++(--iter);
				*iter = std::move(val);
			} else {
				resize(Block_count + 10);
				++(--iter);
				*iter = std::move(val);
			}
		}

		value_type& operator [] ( size_t index ) {
			if ( index >= Size * block_type::num_elements )
				expand_by( index / block_type::num_elements - Size + 1 );

			const auto block_index = index / block_type::num_elements;
			const auto value_index = index % block_type::num_elements;
			return elems[block_index][value_index];
		}

		const value_type& operator [] ( size_t index ) const {
			const auto block_index = index / block_type::num_elements;
			const auto value_index = index % block_type::num_elements;
			return elems[block_index][value_index];
		}


		void resize( size_t new_block_count ) {
			std::unique_ptr<block_type[]> tmp =
				std::make_unique<block_type[]>(new_block_count);

			for ( size_t i = 0; i < new_block_count && i < Block_count; i++ )
				tmp[i] = std::move( elems[i] );

			Block_count = new_block_count;
			elems = std::move(tmp);
		}

		void expand_by( size_t blocks ) {
			if (Size == Block_count)
				resize( Block_count + blocks );

			for (; blocks > 0; --blocks) {
				elems[Size++] = block_type( value_type() );
			}
		}

		iterator find_space() const noexcept {
			auto iter = begin();
			auto last = end();

			for (; iter != last; ++iter) {
				if (*iter == value_type())
					return iter;
			}

			return last;
		}
		
		constexpr size_t size() const noexcept { return end() - begin(); }

		constexpr iterator begin() const noexcept {
			return elems[0].begin();
		}

		constexpr iterator begin() noexcept {
			return elems[0].begin();
		}
		
		constexpr reverse_iterator rend() const noexcept {
			return elems[0].rend();
		}

		constexpr reverse_iterator rend() noexcept {
			return elems[0].rend();
		}

		constexpr iterator iterator_at( size_t index ) noexcept {
			return begin() + index;
		}

		constexpr iterator iterator_at( size_t index ) const noexcept {
			return begin() + index;
		}

		constexpr reverse_iterator riterator_at( size_t index ) noexcept {
			return rbegin() + index;			 			  
		}										 			  
												 			  
		constexpr reverse_iterator riterator_at( size_t index ) const noexcept {
			return rbegin() + index;
		}

		constexpr iterator end() const noexcept {
			return elems[Size - 1].end();
		}

		constexpr iterator end() noexcept {
			return elems[Size - 1].end();
		}
		
		constexpr reverse_iterator rbegin() const noexcept {
			return elems[Size - 1].rbegin();
		}

		constexpr reverse_iterator rbegin() noexcept {
			return elems[Size - 1].rbegin();
		}
	};

	template < typename T >
	class data_manager <T, duo_node<T>> {
	public:
		using value_type = T;
		using node_type = duo_node<T>;

		using iterator = iterators::bi_traverse_iterator<T>;

	private:
		size_t Size = 10;
		node_type* elems = nullptr;
		iterator Begin;

	public:

		data_manager() : Size(10), elems( new node_type[Size] ), Begin(&elems[0]) {
			init( elems, Size );
		}

		data_manager( data_manager&& dm ) : Size(dm.size), elems(dm.elems) {
			dm.elems = nullptr;
			Begin = dm.Begin;
		}

		data_manager& operator = (data_manager&& dm) {
			Size = dm.size;
			delete[] elems;
			elems = dm.elems;
			dm.elems = nullptr;
			Begin = dm.Begin;
			return *this;
		}

		data_manager(const data_manager&) = delete;
		data_manager& operator = (const data_manager&) = delete;


		void insert( const value_type& val, size_t index ) {
			if (index >= Size)
				resize( Size + 10 );

			iterator iter = begin();
			for (; index > 0; index-- )
				iter++;

			insert( val, iter );
		}

		void insert( value_type&& val, size_t index ) {
			if (index >= Size)
				resize( Size + 10 );

			iterator iter = begin();
			for (; index > 0; index--)
				iter++;

			insert( std::forward<value_type>(val), iter );
		}

		void insert( const value_type& val, iterator pos ) {
			size_t index = find_space();
			if (index == Size) {
				size_t iter_index = 0;
				while (pos != Begin) { iter_index++; pos--; }
				resize(Size + 10);
				pos = iterator( std::addressof(elems[iter_index]) );
			}

			if ( *Begin != node_type() && pos != Begin ) {
				iterator prev = pos;
				--prev;
				link_nodes(prev, iterator( std::addressof(elems[index]) ));
				link_nodes( iterator( std::addressof(elems[index]) ), *pos);
			} else if ( *Begin != node_type() && pos == Begin ) {
				link_nodes(elems[index], *pos);
				Begin = iterator( std::addressof(elems[index]) );
				Begin->prev = nullptr;
			}

			elems[index].value = val;
		}

		void insert( value_type&& val, iterator pos ) {
			size_t index = find_space();
			if (index == Size) {
				size_t iter_index = 0;
				while (pos != Begin) { iter_index++; pos--; }
				resize(Size + 10);
				pos = iterator( std::addressof(elems[iter_index]) );
			}

			if ( *Begin.get() != node_type() && pos != Begin ) {
				iterator prev = pos;
				--prev;
				link_nodes( prev, iterator( std::addressof(elems[index]) ) );
				link_nodes( iterator( std::addressof(elems[index]) ), pos );
			} else if (pos == Begin && *Begin.get() != node_type()) {
				link_nodes( elems[index], *pos.get() );
				Begin = iterator( std::addressof(elems[index]) );
				Begin->prev = nullptr;
			}

			elems[index].value = std::move(val);
		}

		value_type& at( size_t index ) {
			iterator iter = Begin;
			for (size_t i = 0; i < index && iter != end(); i++)
				++iter;
			assert(iter.get() != nullptr);
			return iter->value;
		}

		const value_type& at( size_t index ) const {
			iterator iter = Begin;
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
					Begin = next;
				*pos.get() = node_type();
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
			return node >= elems && node < elems + Size;
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

			Size = new_size;
			delete[] elems;
			elems = tmp;
			Begin = iterator( std::addressof(tmp[0]) );
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
			for (size_t i = 0; i < Size; i++) {
				if (elems[i] == node_type())
					return i;
			}

			return Size;
		}

		size_t size() const { return Size; }

		iterator begin() const {
			//check_linkage(elems, _size);
			return iterator(Begin);
		}

		iterator begin() {
			//check_linkage(elems, _size);
			return iterator(Begin);
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