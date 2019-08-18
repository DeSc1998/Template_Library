#pragma once



namespace ds {

	// a more readable alias of a function pointer
	template < typename Ret, typename ... Args >
	using function_pointer = Ret(*)( Args... );

	// only reads values
	template < typename T >
	using read_pointer = const T *;

	// not allowed to change address
	// ( you probably should use a reference instead )
	template < typename T >
	using const_pointer = T * const;

	template < typename T >
	using const_read_pointer = const T * const;


	namespace traits {



	}

}