#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <utility>
#include <ostream>
#include <math.h>

namespace ds {

	template < size_t Base = 10 >
	class integer {
		bool is_negativ = false;
		std::vector<size_t> digets;
		std::wstring symbols = L"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

		static void distribute(integer& n) {
			size_t rest = 0;

			for (auto&& m : n.digets) {
				m += rest;
				rest = m / Base;
				m = m % Base;
			}

			if (rest > 0) {
				n.digets.emplace_back(rest);

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
				digets.emplace_back(value % Base);
				value /= Base;
			}
		}

		template < size_t Size >
		integer(size_t(&value)[Size]) {
			digets.reserve(Size);
			for (int i = 0; i < Size; i++)
				digets[i] = value[i];

			distribute(*this);
		}

		integer(std::vector<size_t> v) {
			using namespace std;
			copy(begin(v), end(v), begin(digets));
			distribute(*this);
		}

		integer(long long value, const std::wstring& s) : symbols(s) {
			if (value < 0) {
				value = -value;
				is_negativ = !is_negativ;
			}

			while (value != 0) {
				digets.emplace_back(value % Base);
				value /= Base;
			}
		}

		template < size_t Size >
		integer(size_t(&value)[Size], const std::wstring& s) : symbols(s) {
			digets.reserve(Size);
			for (int i = 0; i < Size; i++)
				digets[i] = value[i];

			distribute(*this);
		}

		integer(std::vector<size_t> v, const std::wstring& s) :
			symbols(s),
			digets(v)
		{
			distribute(*this);
		}

		integer(const integer& n) :
			is_negativ(n.is_negativ),
			digets(n.digets),
			symbols(n.symbols)
		{ }

		integer(integer&& n) noexcept {
			digets = std::move(n.digets);
			symbols = std::move(n.symbols);
			is_negativ = n.is_negativ;
		}

		integer& operator = (long long value) {
			if (value < 0) {
				value = -value;
				is_negativ = !is_negativ;
			}

			digets.clear();
			do {
				digets.emplace_back(value % Base);
				value /= Base;
			} while (value != 0);
			distribute(*this);

			return *this;
		}

		integer& operator = (const integer& value) {
			using namespace std;
			digets.clear();
			copy(begin(value.digets), end(value.digets), begin(digets));
			copy(begin(value.symbols), end(value.symbols), begin(symbols));
			is_negativ = value.is_negativ;

			distribute(*this);
			return *this;
		}

		integer& operator = (integer&& value) noexcept {
			digets.clear();
			digets = std::move(value.digets);
			symbols = std::move(value.symbols);
			is_negativ = value.is_negativ;

			distribute(*this);
			return *this;
		}

		// casting number<Base> to a diffenrent Type

		// implicit cast
		template < size_t B >
		operator integer<B>() const {
			integer<B> out;
			out.digets.reserve(digets.capacity());
			out.digets.resize(digets.size());

			const double base_factor = std::log((double)Base / (double)B);
			double old_rest = 0.0;

			for (size_t i = this->digets.size() - 1; i > 0; i--) {
				double a_i = std::log((double)this->digets[i]);
				double value = std::exp(i * base_factor + a_i);
				out.digets[i] = (size_t)(value + old_rest * B);
				old_rest = value + old_rest * B - out.digets[i];
			}

			double value = std::exp(std::log((double)digets[0])) + old_rest * B;
			out.digets[0] = (size_t)value;

			value -= (double)out.digets[0];
			if (value >= 0.5) {
				//std::cout << value << '\n';
				out.digets[0] += (size_t)std::round(value);
			}

			integer<B>::distribute(out);

			return out;
		}

		// explicit cast
		template < typename T >
		explicit operator T() const {
			T exp = 1, out = 0;

			for (auto n : digets) {
				out += exp * n;
				exp *= Base;
			}

			if (is_negativ)
				out = -out;

			return out;
		}

		explicit operator std::wstring() const {
			std::wstring out = L"";

			for (auto n : digets)
				out.insert(std::begin(out), symbols[n]);

			while (out[0] == L'0' && out.size() > 1)
				out.erase(0, 1);

			if (is_negativ)
				out.insert(std::begin(out), L'-');

			return out;
		}

		explicit operator std::string() const {
			std::string out = "";

			for (auto n : digets)
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
				auto [_, max] = std::minmax(digets.size(), n.digets.size());
				size_t num_size = max;

				tmp.digets.reserve(num_size + 1);
				tmp.digets.resize(num_size);

				auto this_iter = digets.begin();
				auto other_iter = n.digets.begin();
				auto temp_iter = tmp.digets.begin();

				while (this_iter < digets.end() || other_iter < n.digets.end()) {
					if (this_iter >= digets.end())
						*temp_iter = *other_iter;
					else if (other_iter >= n.digets.end())
						*temp_iter = *this_iter;
					else
						*temp_iter = (*this_iter) + (*other_iter);

					if (this_iter != digets.end())
						++this_iter;

					if (other_iter != n.digets.end())
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
			size_t size = std::minmax(digets.size(), n.digets.size()).second;

			for (size_t i = 0; i < size; i++) {
				old_carry = new_carry;
				new_carry = 0;
				if (tmp.digets[i] == 0 && i == size - 1) {
					tmp.is_negativ = !tmp.is_negativ;
				}
				else if ((long long)tmp.digets[i] - (long long)(n.digets[i] + old_carry) < 0) {
					(tmp.digets[i] += Base) -= (n.digets[i] + old_carry);
					new_carry++;
				}
				else {
					tmp.digets[i] -= (n.digets[i] + old_carry);
				}
			}
			return tmp;
		}

		integer operator * (long long n) const {
			integer tmp = *this;

			for (auto&& m : tmp.digets)
				m *= n;

			distribute(tmp);
			return tmp;
		}

		integer operator * (const integer& n) const {
			integer tmp;
			auto this_size = digets.size();
			auto other_size = n.digets.size();
			tmp.digets.reserve(digets.capacity() + n.digets.capacity());
			tmp.digets.resize(this_size + other_size);

			std::fill(tmp.digets.begin(), tmp.digets.end(), 0);

			for (size_t i = 0; i < this_size; ++i) {
				for (size_t j = 0; j < other_size; ++j) {
					tmp.digets[i + j] += digets[i] * n.digets[j];
				}
			}

			tmp.is_negativ = is_negativ ^ n.is_negativ;

			distribute(tmp);
			return tmp;
		}

	private:
		template < size_t B >
		friend class integer;
	};

	// numder types provided with a default set of symbols
	using Int = integer<>;

	using int_bi = integer<2>;
	using int_qua = integer<4>;
	using int_oct = integer<8>;
	using int_dec = integer<>;
	using int_hex = integer<16>;
	using int_32 = integer<32>;
	using int_max = integer<36>;

	// for calculations recommanded only
	// base exits default set of symbols
	using int_64 = integer<64>;
	using int_128 = integer<128>;
	using int_256 = integer<256>;
	using int_1024 = integer<1024>;

	/*template < size_t Base >
	std::wostream& operator << (std::wostream& o, integer<Base>& n) {
		return (o << static_cast<std::wstring>(n));
	}

	template < size_t Base >
	std::ostream& operator << (std::ostream& o, integer<Base>& n) {
		auto s = static_cast<std::string>(n);
		return o.write(s.c_str(), s.size());
	}*/

	template < size_t Base, typename T >
	std::basic_ostream<T>& operator << (std::basic_ostream<T>& str, const integer<Base>& n) {
		auto s = static_cast<std::basic_string<T>>(n);
		return str.write( s.c_str(), s.size() );
	}

}