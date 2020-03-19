#pragma once

#include <iostream>
#include <utility>

namespace ds {
	template < typename T >
	struct is_range_t {
		template < typename >
		struct SFINAE : std::true_type {};

		// check for begin
		template < typename U >
		static auto test_begin(int) ->
			SFINAE< decltype(std::declval<U>().begin()) >;

		template < typename U >
		static auto test_begin(long) ->
			std::false_type;

		// check for end
		template < typename U >
		static auto test_end(int) ->
			SFINAE< decltype(std::declval<U>().end()) >;

		template < typename U >
		static auto test_end(long) ->
			std::false_type;

		using begin = decltype(test_begin<T>(0));
		using end = decltype(test_end<T>(0));

		static constexpr bool value = begin::value && end::value;
	};

	template < typename T >
	constexpr bool is_range = is_range_t<T>::value;

	template < typename T >
	struct is_reverse_range_t {
		template < typename >
		struct SFINAE : std::true_type {};

		// check for begin
		template < typename U >
		static auto test_rbegin(int) ->
			SFINAE< decltype(std::declval<U>().rbegin()) >;

		template < typename U >
		static auto test_rbegin(long) ->
			std::false_type;

		// check for end
		template < typename U >
		static auto test_rend(int) ->
			SFINAE< decltype(std::declval<U>().rend()) >;

		template < typename U >
		static auto test_rend(long) ->
			std::false_type;

		using rbegin = decltype(test_rbegin<T>(0));
		using rend = decltype(test_rend<T>(0));

		static constexpr bool value = rbegin::value && rend::value && is_range<T>;
	};

	template < typename T >
	constexpr bool is_reverse_range = is_reverse_range_t<T>::value;

	template <
		typename Container,
		typename = std::enable_if_t< is_range<Container> >
	>
		struct range {
		using container_type = Container;
		using iterator_type = typename container_type::iterator;

		iterator_type begin, end;

		range() = delete;

		explicit range(iterator_type begin, iterator_type end) :
			begin(begin), end(end)
		{}

		range(const container_type& con) :
			begin(con.begin()),
			end(con.end())
		{}
	};

	template <
		typename Container,
		typename = std::enable_if_t< is_reverse_range<Container> >
	>
		struct reverse_range {
		using container_type = Container;
		using iterator_type = typename container_type::reverse_iterator;

		iterator_type rbegin, rend;

		reverse_range() = default;

		explicit reverse_range(iterator_type rbegin, iterator_type rend) :
			rbegin(rbegin), rend(rend)
		{}

		reverse_range(const container_type& con) :
			rbegin(con.rbegin()),
			rend(con.rend())
		{}
	};

	template <
		typename Container,
		typename = std::enable_if_t< is_range<Container> >
	>
		inline constexpr range<Container> make_range(const Container& con) {
		return range<Container>(con);
	}

	template <
		typename Container,
		typename = std::enable_if_t< is_reverse_range<Container> >
	>
		inline constexpr reverse_range<Container> make_reverse_range(
			const Container& con
			) {
		return reverse_range<Container>(con);
	}

	template <
		typename Con,
		typename = std::enable_if_t< is_range<Con> >
	>
		inline std::ostream& operator << (std::ostream& stream, const Con& con)
	{
		const auto range = make_range(con);
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
