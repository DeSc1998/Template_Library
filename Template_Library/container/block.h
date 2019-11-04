#pragma once

#include <array>
#include <memory>
#include <cassert>
#include <iterator>
#include <algorithm>

namespace ds {

	template < typename T, size_t Block_size = 0x800 >
	class block {
	public:
		static constexpr size_t num_elem   = Block_size / sizeof(T);
		static constexpr size_t Size       = num_elem * sizeof(T);
		
		using value_type      = T;
		using reference       = value_type&;
		using const_reference = const value_type&;
		using pointer         = value_type*;

		using array_type = std::array<value_type, num_elem>;
		using iterator   = typename array_type::iterator;

	private:
		std::unique_ptr<array_type> elems;

	public:
		block() = default;

		block( const value_type& val ) : 
			elems( std::make_unique<array_type>() ) 
		{
			std::fill( elems->begin(), elems->end(), val );
		}

		block( const block& other ) :
			elems( std::make_unique<array_type>() ) 
		{
			std::copy( other.elems->begin(), other.elems->end(), elems->begin() );
		}

		block(block&&) = default;

		block& operator = (const block& other) {
			std::copy( other.elems->begin(), other.elems->end(), elems->begin() );
			return *this;
		}

		block& operator = (block&&) = default;


		reference operator [] (size_t index) noexcept {
			return (*elems)[index];
		}

		const_reference operator [] (size_t index) const noexcept {
			return (*elems)[index];
		}


		constexpr auto size() const noexcept {
			return elems->size();
		}

		static constexpr auto block_size() noexcept {
			return Size;
		}


		constexpr auto begin() noexcept {
			return elems->begin();
		}
		
		constexpr auto begin() const noexcept {
			return elems->begin();
		}

		constexpr auto rbegin() noexcept {
			return elems->rbegin();
		}

		constexpr auto rbegin() const noexcept {
			return elems->rbegin();
		}

		constexpr auto end() noexcept {
			return elems->end();
		}

		constexpr auto end() const noexcept {
			return elems->end();
		}

		constexpr auto rend() noexcept {
			return elems->rend();
		}

		constexpr auto rend() const noexcept {
			return elems->rend();
		}
	};


	template < typename T, size_t Size >
	class block_iterator {
	public:
		using iterator_category = std::random_access_iterator_tag;

		using block_type = block<T, Size>;

		using Iter            = typename block_type::iterator;
		using value_type      = typename block_type::value_type;
		using reference       = typename block_type::reference;
		using const_reference = typename block_type::const_reference;

	private:
		Iter block_pos;
		block_type* block = nullptr;

	public:
		block_iterator() = default;

		block_iterator( Iter iter, block_type* ptr) :
			block_pos(iter),
			block(ptr)
		{}

		block_iterator(const block_iterator& iter) :
			block_pos(iter.block_pos),
			block(iter.block)
		{}

		block_iterator& operator = ( const block_iterator& other ) noexcept {
			block_pos = other.block_pos;
			block = other.block;

			return *this;
		}

		reference operator * () {
			return *block_pos;
		}

		const_reference operator * () const {
			return *block_pos;
		}

		block_iterator& operator ++ () {
			if ( block_pos + 1 == block->end() ) {
				++block;
				block_pos = block->begin();
			} else {
				++block_pos;
			}

			return *this;
		}

		block_iterator operator ++ (int) {
			auto prev = *this;

			if ( block_pos + 1 == block->end() ) {
				++block;
				block_pos = block->begin();
			} else {
				++block_pos;
			}

			return prev;
		}

		block_iterator& operator -- () {
			if ( block_pos == block->begin() ) {
				--block;
				block_pos = block->end() - 1;
			} else {
				--block_pos;
			}

			return *this;
		}

		block_iterator operator -- (int) {
			auto prev = *this;

			if ( block_pos == block->begin() ) {
				--block;
				block_pos = block->end() - 1;
			} else {
				--block_pos;
			}

			return prev;
		}

		constexpr block_iterator operator + ( size_t val ) const noexcept {
			const auto block_off = val / block->size();
			const auto value_off = val % block->size();

			return block_iterator( block_pos + value_off, block + block_off );
		}
		
		constexpr block_iterator operator - ( size_t val ) const noexcept {
			const auto block_off = val / block->size();
			const auto value_off = val % block->size();

			return block_iterator( block_pos - value_off, block - block_off );
		}

		constexpr size_t operator - ( const block_iterator& other ) const {
			const auto front_dist = std::distance( block_pos, block->end() );
			const auto back_dist = std::distance( other.block->begin(), other.block_pos );
			const auto rest_dist = block != other.block ? 
				(std::distance( other.block, block ) - 1) * block->size() : 0;

			return front_dist + back_dist + rest_dist;
		}


		block_iterator& operator += ( size_t val ) noexcept {
			block     += val / block->size();
			block_pos += val % block->size();

			return *this;
		}

		block_iterator& operator -= ( size_t val ) noexcept {
			block     -= val / block->size();
			block_pos -= val % block->size();

			return *this;
		}

		bool operator == ( const block_iterator& other ) const noexcept {
			return block_pos == other.block_pos && block == other.block;
		}

		bool operator != ( const block_iterator& other ) const noexcept {
			return !( *this == other );
		}
	};


	template < typename T, size_t Size = block<T>::block_size() >
	constexpr block<T, Size>&& make_block( const T& value = T{} ) {
		return block<T, Size>(value);
	}

}