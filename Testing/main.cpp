
#include <iostream>

#include "container/stack.h"

void stack_test();

int main() {

	stack_test();

	std::cin.get();
	return 0;
}


void stack_test() {
	ds::stack<int> st;

	// push test
	st.push(10);
	st.push(14);
	st.push(21);
	st.push(35);
	st.push(50);
	st.push(2);
	st.push(55);

	// iterator test
	for (auto iter = st.begin(); iter != st.end(); ++iter)
		std::cout << iter->value << ", ";

	std::cout << "\n\n";

	// pop and top test
	while (!st.is_empty()) {
		std::cout << st.top() << ", ";
		st.pop();
	}

	// resize test
	std::cout << "\n\n";
	for (int i = 1; i < 51; i++)
		st.push(i * 2);

	// foreach test
	for (auto node : st)
		std::cout << node.value << ", ";
}
