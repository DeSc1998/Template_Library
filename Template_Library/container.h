#pragma once

#include <iostream>
#include <utility>

#include "container/mono_list.h"
#include "container/list.h"
#include "container/stack.h"
#include "container/vector.h"


namespace ds {

	

	template < typename T >
	struct is_range_t {

		template < typename >
		struct SFINAE : std::true_type {};

		// check for begin
		template < typename U >
		static auto test_begin( int ) ->
			SFINAE< decltype( std::declval<U>().begin() ) >;

		template < typename U >
		static auto test_begin(long) ->
			std::false_type;

		// check for end
		template < typename U >
		static auto test_end( int ) ->
			SFINAE< decltype( std::declval<U>().end() ) >;

		template < typename U >
		static auto test_end(long) ->
			std::false_type;

		using begin = decltype( test_begin<T>(0) );
		using end = decltype( test_end<T>(0) );

		static constexpr bool value = begin::value && end::value;
	};

	template < typename T >
	constexpr bool is_range = is_range_t<T>::value;


	template <
		typename Container,
		typename = std::enable_if_t< is_range<Container> >
	>
	struct range {

		using container_type = Container;
		using iterator_type = typename container_type::iterator;

		iterator_type begin, end;

		range() = delete;

		explicit range( iterator_type begin, iterator_type end ) : 
			begin(begin), end(end) 
		{}

		range( const Container& con ) :
			begin( con.begin() ),
			end( con.end() )
		{}
	};


	template <
		typename Container,
		typename = std::enable_if_t< is_range<Container> >
	>
	inline constexpr range<Container> make_range( const Container& con ) {
		return range<Container>( con );
	}

	template <
		typename Con,
		typename = std::enable_if_t<
			std::is_constructible_v<range<Con>, Con>
		>
	>
	inline std::ostream& operator << ( std::ostream& stream, const Con& con )
	{
		const auto range = make_range( con );
		auto iter = range.begin;

		do {
			if (iter == range.begin)
				stream << '[';

			stream << *iter;

			if (++iter == range.end)
				stream << ']';
			else
				stream << ';';
		} while (iter != range.end);

		return stream;
	}

}

