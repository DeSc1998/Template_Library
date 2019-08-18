
#include "Types.h"
#include "container/stack.h"

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
		std::cout << iter->value << '\n';

	std::cout << "\n\n";

	while ( !st.is_empty() ) {
		std::cout << st.top() << '\n';
		st.pop();
	}

	std::cout << '\n';
	for (int i = 1; i < 101; i++)
		st.push( i * 2 );

	for (auto iter = st.begin(); iter != st.end(); ++iter)
		std::cout << iter->value << '\n';

	std::cin.get();
	return 0;
}