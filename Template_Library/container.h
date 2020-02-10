#pragma once

#include <iostream>

#include "container/mono_list.h"
#include "container/list.h"
#include "container/stack.h"
#include "container/vector.h"


namespace ds {

	template <
		typename Container,
		typename = std::enable_if_t<
			std::is_class_v<Container>
		>
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
		typename = std::enable_if_t<
			std::is_class_v<Container>
		>
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

