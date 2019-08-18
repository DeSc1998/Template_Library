#pragma once

namespace ds {

	// needs some testing ...
	namespace typelist {

		// basic typelist
		template < typename ... Elem >
		class list;


		template < typename List, bool Is_Empty = is_empty_v<List> >
		class unfold_t;

		template < typename ... Elems >
		class unfold_t < list<Elems...>, false > {
		public:
			using Type = Elems...;
		};

		template <>
		class unfold_t < list<>, true > {
		public:
			using Type = void;
		};

		template < typename List >
		using unfold = typename unfold_t<List>::Type;


		template < typename List >
		class front_t;

		template < typename Head, typename ... Tail >
		class front_t < list< Head, Tail... > > {
		public:
			using Type = Head;
			using Rest = list<Tail...>;
		};

		template < typename List >
		using front = typename front_t<List>::Type;

		template < typename List >
		using pop_front = typename front_t<List>::Rest;


		template < typename List >
		class back_t;

		template < typename ... Front, typename Back >
		class back_t < list< Front..., Back > > {
		public:
			using Type = Back;
			using Rest = list<Front...>;
		};

		template < typename List >
		using back = typename back_t<List>::Type;

		template < typename List >
		using pop_back = typename back_t<List>::Rest;


		template < typename List >
		class is_empty {
		public:
			static constexpr bool value = false;
		};

		template <>
		class is_empty < list<> > {
		public:
			static constexpr bool value = true;
		};

		template < typename List >
		constexpr bool is_empty_v = is_empty<List>::value;


		template < typename List, typename Elem >
		class push_back_t;

		template < typename ... Elems, typename Elem >
		class push_back_t < list<Elems...>, Elem > {
		public:
			using Type = list<Elems..., Elem>;
		};

		template < typename List, typename Elem >
		using push_back = typename push_back_t<List, Elem>::Type;


		template < typename List, typename Elem >
		class push_front_t;

		template < typename ... Elems, typename Elem >
		class push_front_t < list<Elems...>, Elem > {
		public:
			using Type = list<Elem, Elems...>;
		};

		template < typename List, typename Elem >
		using push_front = typename push_front_t<List, Elem>::Type;


		template < typename List, bool Is_Empty = is_empty_v<List> >
		class size_of;

		template < typename List >
		class size_of < List, false > {
		public:
			static constexpr size_t value = size_of< pop_front<List> >::size + 1;
		};

		template <>
		class size_of < list<>, true > {
		public:
			static constexpr size_t value = 0;
		};

		template < typename List >
		constexpr size_t size_of_v = size_of<List>::value;


		// TODO reverse
		template < typename List, bool Is_Empty = is_empty_v<List> >
		class reverse_t;


		template < typename List1, typename List2 >
		class merge_t;

		template < typename ... Elems1, typename ... Elems2 >
		class merge_t < list<Elems1...>, list<Elems2...> > {
		public:
			using Type = list< Elems1..., Elems2... >;
		};

		template < typename List1, typename List2 >
		using merge = typename merge_t<List1, List2>::Type;

	}


	template < int N >
	class Enum {
	public:
		static constexpr int value = N;

		constexpr int operator () () {
			return value;
		}
	};


	template < int N1, int N2 >
	constexpr Enum<N1> enum_cast( Enum<N2> ) {
		return Enum<N1>();
	}

}

#define type_to_string( Type ) #Type

#define enum_value( Value_name, Value ) \
	Value_name = Value

#define enum_from_list( Enum_name, List )   \
	enum class Enum_name {                  \
		enum_value(                         \
			type_to_string( ds::typelist::unfold<List> ), \
			ds::typelist::unfold<List>()()  \
		) ...								\
	}; 