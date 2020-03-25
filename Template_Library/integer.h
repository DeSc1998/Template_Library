#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <ostream>
#include <math.h>

#include "range.h"

namespace ds {

	template < size_t Base = 10 >
	class integer {
		bool is_negativ = false;
		std::vector<size_t> digits;
		std::wstring symbols = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

		static void distribute(integer& n) {
			size_t rest = 0;

			for (auto&& m : n.digits) {
				m += rest;
				rest = m / Base;
				m %= Base;
			}

			if (rest > 0) {
				n.digits.emplace_back(rest);

				if (rest >= Base)
					distribute(n);
			}
		}

	public:
		integer() = default;

		integer(long long value) {
			if (value < 0) {
				value = -value;
				is_negativ = !is_negativ;
			}

			while (value != 0) {
				digits.emplace_back(value % Base);
				value /= Base;
			}
		}

		template < size_t Size >
		integer(size_t(&value)[Size]) {
			digits.reserve(Size);
			for (int i = 0; i < Size; i++)
				digits[i] = value[i];

			distribute(*this);
		}

		integer(std::vector<size_t> v) {
			using namespace std;
			copy(begin(v), end(v), begin(digits));
			distribute(*this);
		}

		integer(std::string_view str) {
			using namespace std;
			auto set = symbols.substr(0, Base);

			for (auto c : str)
				digits.emplace(begin(digits), set.find(c));

			distribute(*this);
		}

		integer(std::wstring_view str) {
			using namespace std;
			auto set = symbols.substr(0, Base);

			for (auto c : str)
				digits.emplace(begin(digits), set.find(c));

			distribute(*this);
		}

		integer(long long value, const std::wstring& s) : symbols(s) {
			if (value < 0) {
				value = -value;
				is_negativ = !is_negativ;
			}

			while (value != 0) {
				digits.emplace_back(value % Base);
				value /= Base;
			}
		}

		template < size_t Size >
		integer(size_t(&value)[Size], const std::wstring& s) : symbols(s) {
			digits.reserve(Size);
			for (int i = 0; i < Size; i++)
				digits[i] = value[i];

			distribute(*this);
		}

		integer(std::vector<size_t> v, const std::wstring& s) :
			symbols(s),
			digits(v)
		{
			distribute(*this);
		}

		integer(const integer& n) :
			is_negativ(n.is_negativ),
			digits(n.digits),
			symbols(n.symbols)
		{ }

		integer(integer&& n) noexcept {
			digits = std::move(n.digits);
			symbols = std::move(n.symbols);
			is_negativ = n.is_negativ;
		}

		integer& operator = (long long value) {
			if (value < 0) {
				value = -value;
				is_negativ = !is_negativ;
			}

			digits.clear();
			do {
				digits.emplace_back(value % Base);
				value /= Base;
			} while (value != 0);
			distribute(*this);

			return *this;
		}

		integer& operator = (const integer& value) {
			using namespace std;
			digits.clear();
			copy(begin(value.digits), end(value.digits), begin(digits));
			copy(begin(value.symbols), end(value.symbols), begin(symbols));
			is_negativ = value.is_negativ;

			distribute(*this);
			return *this;
		}

		integer& operator = (integer&& value) noexcept {
			digits.clear();
			digits = std::move(value.digits);
			symbols = std::move(value.symbols);
			is_negativ = value.is_negativ;

			distribute(*this);
			return *this;
		}

		// casting number<Base> to a diffenrent Type

		// implicit cast
		template < size_t B >
		operator integer<B>() const {
			using std::log;
			using std::exp;

			integer<B> out;
			out.digits.reserve(digits.capacity());
			out.digits.resize(digits.size());

			const double base_factor = log((double)Base / (double)B);

			auto converter = [index = digits.size() - 1, base_factor, rest = 0.0]
			(size_t a_i) mutable->size_t {
				// a_i * Base^i = b_i * B^i
				// => b_i = a_i * (Base / B)^i
				//    b_i = a_i * exp(i * ln(Base / B))

				auto b_i = exp(index-- * base_factor + log((double)a_i));
				size_t new_value = (size_t)(b_i + rest * B);
				rest = b_i + rest * B - new_value;

				if (index + 1 == 0 && rest >= 0.5) {
					++new_value;
					rest = 0.0;
				}

				return new_value;
			};

			std::transform(digits.rbegin(), digits.rend(), out.digits.rbegin(), converter);

			integer<B>::distribute(out);

			return out;
		}

		// explicit cast
		template < typename T >
		explicit operator T() const {
			T exp = 1, out = 0;

			for (auto n : digits) {
				out += exp * n;
				exp *= Base;
			}

			if (is_negativ)
				out = -out;

			return out;
		}

		explicit operator std::wstring() const {
			std::wstring out = L"";

			for (auto n : digits)
				out.insert(std::begin(out), symbols[n]);

			while (out[0] == L'0' && out.size() > 1)
				out.erase(0, 1);

			if (is_negativ)
				out.insert(std::begin(out), L'-');

			return out;
		}

		explicit operator std::string() const {
			std::string out = "";

			for (auto n : digits)
				out.insert(std::begin(out), (char)symbols[n]);

			while (out[0] == '0' && out.size() > 1)
				out.erase(0, 1);

			if (is_negativ)
				out.insert(std::begin(out), '-');

			return out;
		}
		// end of casting

		void set_symbols(const std::wstring& s) {
			symbols = s;
		}

		auto digit_count() const noexcept {
			return digits.size();
		}

		static integer negativ_of(const integer& n) {
			integer out = n;
			out.is_negativ = !out.is_negativ;
			return out;
		}

		integer operator + (const integer& n) const {
			if (n.is_negativ) {
				return *this - negativ_of(n);
			}
			else if (this->is_negativ) {
				return n - negativ_of(*this);
			}
			else {
				integer tmp;
				auto size = std::max(digits.size(), n.digits.size());

				tmp.digits.reserve(size + 1);
				tmp.digits.resize(size);

				auto this_iter = digits.begin();
				auto other_iter = n.digits.begin();
				auto temp_iter = tmp.digits.begin();

				while (this_iter < digits.end() || other_iter < n.digits.end()) {
					if (this_iter >= digits.end())
						*temp_iter = *other_iter;
					else if (other_iter >= n.digits.end())
						*temp_iter = *this_iter;
					else
						*temp_iter = (*this_iter) + (*other_iter);


					if (this_iter != digits.end())
						++this_iter;

					if (other_iter != n.digits.end())
						++other_iter;

					++temp_iter;
				}

				distribute(tmp);
				return tmp;
			}
		}

		integer operator - (const integer& n) const {
			size_t old_carry = 0, new_carry = 0;
			integer tmp = *this;
			size_t size = std::max(digits.size(), n.digits.size());

			for (size_t i = 0; i < size; i++) {
				old_carry = new_carry;
				new_carry = 0;
				if (tmp.digits[i] == 0 && i == size - 1) {
					tmp.is_negativ = !tmp.is_negativ;
				}
				else if ((long long)tmp.digits[i] - (long long)(n.digits[i] + old_carry) < 0) {
					(tmp.digits[i] += Base) -= (n.digits[i] + old_carry);
					new_carry++;
				}
				else {
					tmp.digits[i] -= (n.digits[i] + old_carry);
				}
			}
			return tmp;
		}

		integer operator * (long long n) const {
			integer tmp = *this;

			for (auto&& m : tmp.digits)
				m *= n;

			distribute(tmp);
			return tmp;
		}

		integer operator * (const integer& n) const {
			integer tmp;
			auto this_size = digits.size();
			auto other_size = n.digits.size();
			tmp.digits.reserve(digits.capacity() + n.digits.capacity());
			tmp.digits.resize(this_size + other_size);

			std::fill(tmp.digits.begin(), tmp.digits.end(), 0);

			for (size_t i = 0; i < this_size; ++i) {
				for (size_t j = 0; j < other_size; ++j) {
					tmp.digits[i + j] += digits[i] * n.digits[j];
				}
			}

			tmp.is_negativ = is_negativ ^ n.is_negativ;

			distribute(tmp);
			return tmp;
		}

		// TODO: division (operator /)
		integer operator / (const integer& n) const {


			return integer();
		}

	private:
		template < size_t B >
		friend class integer;
	};

	// numder types provided with a default set of symbols
	using Int = integer<>;

	using Int_bi = integer<2>;
	using Int_qua = integer<4>;
	using Int_oct = integer<8>;
	using Int_dec = integer<>;
	using Int_hex = integer<16>;
	using Int_32 = integer<32>;
	using Int_max = integer<36>;

	// for calculations recommanded only
	// base exits default set of symbols
	using Int_64 = integer<64>;
	using Int_128 = integer<128>;
	using Int_256 = integer<256>;
	using Int_1024 = integer<1024>;


	template < size_t Base, typename T >
	std::basic_ostream<T>& operator << (std::basic_ostream<T>& str, const integer<Base>& n) {
		auto s = static_cast<std::basic_string<T>>(n);
		return str.write(s.c_str(), s.size());
	}

}