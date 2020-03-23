
#include <iostream>

#include "container/stack.h"
#include "container/list.h"
#include "container/vector.h"
#include "range.h"
#include "integer.h"

#include "test_funcs.h"

namespace test {

	void stack() {
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


	void list() {
		ds::list<int> list;

		// insertion test (passed)
		list.insert(12);
		list.insert(15);
		list.insert(0);
		list.insert(22, 1);
		list.insert(54, 1);
		list.insert(99);

		// iterator test (passed)
		for (auto iter = list.begin(); iter != list.end(); ++iter) {
			std::cout << iter->value << ", ";
		}
		std::cout << '\n' << list.size() << '\n';

		// erase test (passed)
		list.erase(0);
		list.erase(10);
		list.erase(15);

		// 2nd iterator test (passed)
		std::cout << '\n';
		for (auto iter = list.begin(); iter != list.end(); ++iter) {
			std::cout << iter->value << ", ";
		}
		std::cout << '\n' << list.size() << '\n';

		// resize test (passed)
		//( inserting more elements than the default size )
		for (int i = 0; i < 51; i++)
			list.insert(i * 2);

		// foreach test (passed)
		std::cout << '\n';
		for (auto val : list)
			std::cout << val << ", ";

		std::cout << '\n' << list.size() << '\n' << '\n';

		std::cout << list << "\n\n";

		// index operator test
		std::cout << list[5];
	}


	void vector() {
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

		vec.clear();

		// insertion beyond default size
		for (int i = 0; i < 512; i++) {
			vec.push_back(i * 2);
		}

		for (auto var : vec) {
			std::cout << var << ", ";
		}
		std::cout << '\n';

		std::cout << vec;
	}

	void iterator() {
		ds::vector<int> v;

		for (int i = 3; i < 15; ++i)
			v.push_back(i * 3);

		for (auto iter = v.begin(); iter != v.end(); ++iter) {
			std::cout << *iter << ", ";
			++(--iter);
		}
	}

	void integer() {
		ds::integer<32> i;
		ds::integer j = 10000;
		ds::integer<16> k = j;
		ds::integer<64> l = 200;

		i = 21;

		std::cout << " i <32> = " << i << '\n';
		std::cout << " j <10> = " << j << '\n';
		std::cout << " j <32> = " << (ds::integer<32>)j << '\n';
		std::cout << " k <16> = " << k << '\n';

		// TODO: converiton from large to lower bases is currently brocken
		std::cout << " l <64 -> 20> = " << (ds::integer<20>)k << '\n';
		std::cout << " i + j = " << (i + j) << '\n';
		std::cout << " i * j = " << ds::integer<10>(i * j) << '\n';
		std::cout << " j - k = " << k - j << '\n';



	}

}
