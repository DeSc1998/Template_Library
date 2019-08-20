#pragma once

#include <type_traits>

namespace ds {

	template < typename T, size_t H, bool = std::is_class_v<T> && !std::is_final_v<T> >
	class tuple_impl;

	template < typename T, size_t H >
	class tuple_impl <T, H, false> {
		T value;
	public:
		tuple_impl() = default;

		template < typename U >
		tuple_impl( U&& u ) : value( std::forward<U>(u) ) {}
		
		T&       get()       { return value; }
		const T& get() const { return value; }
	};

	template < typename T, size_t H >
	class tuple_impl <T, H, true> : public T {
	public:
		tuple_impl() = default;

		template < typename U >
		tuple_impl( U&& u ) : value( std::forward<U>(u) ) {}

		T&       get()       { return *this; }
		const T& get() const { return *this; }
	};


	template < typename ... Types >
	class tuple;

	template < typename H, typename ... T >
	class tuple < H, T... > :
		private tuple_impl<H, sizeof...(T)>,
		private tuple<T...>
	{
		using headimpl = tuple_impl<H, sizeof...(T)>;
	public:
		tuple() = default;

		tuple( H&& h, T&& ... t ) : 
			headimpl(std::forward<H>(h)),
			tuple<T...>( std::forward<T>(t)... )
		{}


		H& get_head() {
			return static_cast<headimpl*>(this)->get();
		}

		const H& get_head() const {
			return static_cast<const headimpl*>(this)->get();
		}

		tuple<T...>& get_tail() { return *this; }
		const tuple<T...>& get_tail() const { return *this; }
	};

	template <>
	class tuple <> {};


	template < size_t H, typename T >
	T& get_elem( tuple_impl<T, H>& elem ) {
		return elem.get();
	}

	template < size_t I, typename ... Elems >
	auto get( tuple<Elems...>& t )
			-> decltype( get_elem<sizeof...(Elems) - I - 1>(t) ) {
		return get_elem<sizeof...(Elems)-I-1>(t);
	}

}
