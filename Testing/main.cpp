
#include <iostream>

#include "container.h"

void stack_test();
void list_test();
void vector_test();


int main() {



	stack_test();
	
	std::cout << "\n\n\n";
	
	list_test();
	
	std::cout << "\n\n\n";
	
	vector_test();

	std::cin.get();
	return 0;
}


void stack_test() {
	ds::stack<int> st;

	// push test (passed)
	st.push(10);
	st.push(14);
	st.push(21);
	st.push(35);
	st.push(50);
	st.push(2);
	st.push(55);

	// iterator test (passed)
	for (auto iter = st.begin(); iter != st.end(); ++iter)
		std::cout << *iter << ", ";

	std::cout << "\n\n";

	// pop and top test (passed)
	while (!st.is_empty()) {
		std::cout << st.top() << ", ";
		st.pop();
	}

	// resize test (passed)
	//( inserting more elements than the default size )
	std::cout << "\n\n";
	for (int i = 1; i < 701; i++)
		st.push(i * 2);
  
	// foreach test (passed)
	for (auto value : st)
		std::cout << value << ", ";

	std::cout << "\n" << st << "\n";
}

void list_test() {
	ds::list<int> list;

	// insertion test (passed)
	list.insert(12);
	list.insert(15);
	list.insert(0);
	list.insert(22, 1);
	list.insert(54, 1);
	list.insert(99);

	// iterator test (passed)
	for ( auto iter = list.begin(); iter != list.end(); ++iter ) {
		std::cout << iter->value << ", ";
	}
	std::cout << '\n' << list.size() << '\n';

	// erase test (passed)
	list.erase( 0 );
	list.erase( 10 );
	list.erase( 15 );

	// 2nd iterator test (passed)
	std::cout << '\n';
	for (auto iter = list.begin(); iter != list.end(); ++iter) {
		std::cout << iter->value << ", ";
	}
	std::cout << '\n' << list.size() << '\n';

	// resize test (passed)
	//( inserting more elements than the default size )
	for (int i = 0; i < 51; i++)
		list.insert( i*2 );

	// foreach test (passed)
	std::cout << '\n';
	for (auto val : list)
		std::cout << val << ", ";

	std::cout << '\n' << list.size() << '\n' << '\n';

	std::cout << list << "\n\n";

	// index operator test
	std::cout << list[5];
}

void vector_test() {
	ds::vector<int> vec;

	// insertion test
	vec.insert(15);
	vec.insert(12);

	// iterator test
	for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
		std::cout << *iter << ", ";
	}
	std::cout << '\n';

	vec.insert(19, 1);
	vec.insert(34, 1);

	// iterator test
	for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
		std::cout << *iter << ", ";
	}
	std::cout << '\n';

	vec.push_back(55);
	vec.push_back(76);

	// iterator test
	for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
		std::cout << *iter << ", ";
	}
	std::cout << '\n';

	vec.push_front(100);
	vec.push_front(200);

	// iterator test
	for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
		std::cout << *iter << ", ";
	}
	std::cout << '\n' << '\n';

	// erase test
	vec.erase_at(2);
	vec.erase_at(2);
	vec.erase_at(2);

	vec.erase(10);
	vec.erase(15);
	vec.erase(19);

	// foreach test
	for (auto val : vec) {
		std::cout << val << ", ";
	}
	std::cout << '\n';

	// insertion beyond default size
	for ( int i = 0; i < 51; i++ ) {
		vec.push_back( i*2 );
	}

	for ( auto var : vec) {
		std::cout << var << ", ";
	}
	std::cout << '\n';

	std::cout << vec;
}
