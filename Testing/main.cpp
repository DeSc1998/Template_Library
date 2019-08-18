
#include "Types.h"
#include "stack.h"

#include <iostream>


int main() {
	using ds::stack;

	ds::function_pointer<int> ptr = &main;
	stack<int> st;

	st.push( 10 );
	st.push( 14 );
	st.push( 21 );
	st.push( 35 );
	st.push( 50 );
	st.push( 2 );
	st.push( 55 );

	std::cout << __FUNCSIG__ << '\n';

	for (auto iter = st.begin(); iter != st.end(); ++iter)
		std::cout << *iter << '\n';

	std::cin.get();
	return 0;
}