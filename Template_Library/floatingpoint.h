#pragma once

#include <vector>
#include <string>
#include <utility>
#include <ostream>


template < size_t Base >
class floatingpoint {
	bool is_negativ = false;
	std::vector<size_t> digets_pos;
	std::vector<size_t> digets_neg;
	std::wstring symbols = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	static void distribute(floatingpoint& f) {

	}

public:
	floatingpoint() = default;


};