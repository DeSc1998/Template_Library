
#include "Types.h"
#include "container/stack.h"

#include <iostream>


int main() {

	ds::stack<int> st;

	// push test
	st.push( 10 );
	st.push( 14 );
	st.push( 21 );
	st.push( 35 );
	st.push( 50 );
	st.push( 2 );
	st.push( 55 );

	std::cout << __FUNCSIG__ << '\n';

	// iterator test
	for (auto iter = st.begin(); iter != st.end(); ++iter)
		std::cout << iter->value << '\n';

	std::cout << "\n\n";

	// pop and top test
	while ( !st.is_empty() ) {
		std::cout << st.top() << '\n';
		st.pop();
	}

	// resize test
	std::cout << '\n';
	for (int i = 1; i < 51; i++)
		st.push( i * 2 );

	// foreach test
	for ( auto val : st )
		std::cout << val.value << '\n';



	std::cin.get();
	return 0;
}