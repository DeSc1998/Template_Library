#pragma once

#include <array>
#include <memory>
#include <iterator>
#include <algorithm>

#include "../Types.h"

namespace ds {

	template < typename T, size_t S >
	class block_iterator;

	template < typename T, size_t S >
	class reverse_block_iterator;


	template < typename T, size_t Block_size = 0x800 >
	class block {
	public:
		static constexpr size_t num_elements = Block_size / sizeof(T);
		static constexpr size_t Size         = num_elements * sizeof(T);
		
		using value_type      = T;
		using reference       = value_type&;
		using const_reference = const value_type&;
		using pointer         = value_type*;

		using const_pointer = const_pointer<T>;

		using iterator         = block_iterator<T, Size>;
		using reverse_iterator = reverse_block_iterator<T, Size>;

		friend class block_iterator<T, Size>;
		friend class reverse_block_iterator<T, Size>;

	private:
		std::unique_ptr<value_type[]> elems;

	public:
		block() = default;

		block( const value_type& val ) : 
			elems( std::make_unique<value_type[]>( num_elements ) ) 
		{
			std::fill( elems.get(), elems.get() + num_elements, val );
		}

		block( const block& other ) :
			elems( std::make_unique<value_type[]>( num_elements ) ) 
		{
			std::copy( elems.get(), elems.get() + num_elements, elems.get() );
		}

		block( block&& other ) : elems( std::move(other.elems) ) {}

		block& operator = (const block& other) {
			std::copy( other.elems.get(), other.elems.get() + num_elements, elems.get() );
			return *this;
		}

		block& operator = ( block&& other ) noexcept {
			elems = std::move( other.elems );
			return *this;
		}

	private:
		const_pointer get_begin() const noexcept {
			return &elems[0];
		}

		const_pointer get_rbegin() const noexcept {
			return &elems[num_elements - 1];
		}

	public:
		reference operator [] (size_t index) {
			return elems[index];
		}

		const_reference operator [] (size_t index) const {
			return elems[index];
		}


		constexpr auto size() const noexcept {
			return num_elements;
		}

		static constexpr auto block_size() noexcept {
			return Size;
		}


		constexpr auto begin() noexcept {
			return iterator(0, this);
		}
		
		constexpr auto begin() const noexcept {
			return iterator(0, this);
		}

		constexpr auto rbegin() noexcept {
			return reverse_iterator(0, this);
		}

		constexpr auto rbegin() const noexcept {
			return reverse_iterator(0, this);
		}

		constexpr auto end() noexcept {
			return iterator(num_elements, this);
		}

		constexpr auto end() const noexcept {
			return iterator(num_elements, this);
		}

		constexpr auto rend() noexcept {
			return reverse_iterator(num_elements, this);
		}

		constexpr auto rend() const noexcept {
			return reverse_iterator(num_elements, this);
		}
	};


	template < typename T, size_t S >
	class block_iterator {
	public:
		using block_type      = block<T, S>;
		using size_type       = size_t;

		// iterator types
		using iterator_category = std::random_access_iterator_tag;
		using value_type        = typename block_type::value_type;
		using reference         = typename block_type::reference;
		using const_reference   = typename block_type::const_reference;
		using pointer           = typename block_type::value_type*;
		using difference_type   = ptrdiff_t;

		friend class block<T, S>;

		static constexpr size_t size = block_type::num_elements;

	private:
		size_type offset;
		block_type* block_pos = nullptr;

	public:
		block_iterator() = default;

		block_iterator( size_type off, block_type* ptr) :
			offset(off % size),
			block_pos(ptr + (off / size))
		{}

		block_iterator(const block_iterator&) = default;
		block_iterator(block_iterator&&) = default;
		block_iterator& operator = (const block_iterator&) = default;

		explicit operator reverse_block_iterator<T, S>() {
			return reverse_block_iterator<T, S>( size - offset, block_pos );
		}


		reference operator * () {
			auto Begin = block_pos->get_begin();
			return *(Begin + offset);
		}

		const_reference operator * () const {
			auto Begin = block_pos->get_begin();
			return *(Begin + offset);
		}

		reference operator [] (size_type idx) {
			return *(*this + idx);
		}

		const_reference operator [] (size_type idx) const {
			return *(*this + idx);
		}

		block_iterator& operator ++ () {
			if ( ++offset == size ) {
				++block_pos;
				offset = 0;
			}

			return *this;
		}

		block_iterator operator ++ (int) {
			auto prev = *this;
			++(*this);
			return prev;
		}

		block_iterator& operator -- () {
			if ( --offset >= size ) {
				--block_pos;
				offset = 0;
			}

			return *this;
		}

		block_iterator operator -- (int) {
			auto prev = *this;
			--(*this);
			return prev;
		}

		constexpr block_iterator operator + ( size_type val ) const noexcept {
			const auto block_off = val / size;
			const auto value_off = val % size;
			
			return block_iterator(offset + value_off, block_pos + block_off);
		}
		
		constexpr block_iterator operator - ( size_type val ) const noexcept {
			const auto block_off = val / size;
			const auto value_off = val % size;

			return block_iterator(offset - value_off, block_pos - block_off);
		}

		constexpr difference_type operator - ( const block_iterator& other ) const noexcept {
			const auto offset_dist = offset - other.offset;
			const auto block_diff = block_pos - other.block_pos;
			const auto block_dist = block_diff * size;

			return offset_dist + block_dist;
		}


		block_iterator& operator += ( size_type val ) noexcept {
			return *this = *this + val;
		}

		block_iterator& operator -= ( size_type val ) noexcept {
			return *this = *this - val;
		}

		bool operator == ( const block_iterator& other ) const noexcept {
			return offset == other.offset && block_pos == other.block_pos;
		}

		bool operator != ( const block_iterator& other ) const noexcept {
			return !( *this == other );
		}
	};

	template < typename T , size_t S >
	class reverse_block_iterator {
	public:
		using block_type    = block<T, S>;
		using size_type     = size_t;

		// iterator types
		using iterator_category = std::random_access_iterator_tag;
		using value_type        = typename block_type::value_type;
		using reference         = typename block_type::reference;
		using const_reference   = typename block_type::const_reference;
		using pointer           = typename block_type::pointer;
		using difference_type   = ptrdiff_t;

		friend class block<T, S>;

		static constexpr size_t size = block_type::num_elements;

	private:
		size_type offset;
		block_type* block_pos = nullptr;

	public:
		reverse_block_iterator() = default;

		reverse_block_iterator(size_type off, block_type * ptr) :
			offset(off % size),
			block_pos(ptr - (off / size))
		{}

		reverse_block_iterator(const reverse_block_iterator&) = default;
		reverse_block_iterator(reverse_block_iterator&&) = default;
		reverse_block_iterator& operator = (const reverse_block_iterator&) = default;

		explicit operator block_iterator<T, S>() {
			return block_iterator<T, S>(size - offset, block_pos);
		}


		reference operator * () {
			auto rBegin = block_pos->get_rbegin();
			return *(rBegin - offset);
		}

		const_reference operator * () const {
			auto rBegin = block_pos->get_rbegin();
			return *(rBegin - offset);
		}

		reference operator [] (size_type idx) {
			return *(*this + idx);
		}

		const_reference operator [] (size_type idx) const {
			return *(*this + idx);
		}

		reverse_block_iterator& operator ++ () {
			if (++offset == size) {
				--block_pos;
				offset = 0;
			}

			return *this;
		}

		reverse_block_iterator operator ++ (int) {
			auto prev = *this;
			++(*this);
			return prev;
		}

		reverse_block_iterator& operator -- () {
			if (--offset >= size) {
				++block_pos;
				offset = size - 1;
			}

			return *this;
		}

		reverse_block_iterator operator -- (int) {
			auto prev = *this;
			--(*this);
			return prev;
		}

		constexpr reverse_block_iterator operator + (size_type val) const noexcept {
			const auto block_off = val / size;
			const auto value_off = val % size;

			return reverse_block_iterator(offset + value_off, block_pos - block_off);
		}

		constexpr reverse_block_iterator operator - (size_type val) const noexcept {
			const auto block_off = val / size;
			const auto value_off = val % size;

			return reverse_block_iterator(offset - value_off, block_pos + block_off);
		}

		constexpr difference_type operator - (const reverse_block_iterator& other) const noexcept {
			const auto offset_dist = offset - other.offset;
			const auto block_diff = other.block_pos - block_pos;
			const auto block_dist = block_diff * size;

			return offset_dist + block_dist;
		}


		reverse_block_iterator& operator += (size_type val) noexcept {
			return *this = *this + val;
		}

		reverse_block_iterator& operator -= (size_type val) noexcept {
			return *this = *this - val;
		}

		bool operator == (const reverse_block_iterator & other) const noexcept {
			return offset == other.offset && block_pos == other.block_pos;
		}

		bool operator != (const reverse_block_iterator & other) const noexcept {
			return !(*this == other);
		}
	};


	template < typename T, size_t Size = block<T>::block_size() >
	constexpr block<T, Size>&& make_block( const T& value = T{} ) {
		return block<T, Size>(value);
	}

	template < typename T, size_t Size = block<T>::block_size() >
	auto&& make_block_array(size_t length, const T& value = T{}) {
		auto temp_array = std::make_unique<block<T, Size>>( length );
		for (auto& b : temp_array)
			b = block<T, Size>( value );

		return std::move( temp_array );
	}

}
