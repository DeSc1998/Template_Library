#pragma once

#include <ostream>

#include "data_manager.h"

namespace ds {
	template <
		typename T,
		typename Manager = data_manager<T>
	>
		class vector {
		public:
			using value_type = typename Manager::value_type;

			using iterator = typename Manager::iterator;
			using reverse_iterator = typename Manager::reverse_iterator;
		private:
			Manager data;
			iterator last = data.begin();

		public:
			vector() = default;

			vector(const value_type& val) : data() {
				data[0] = val;
				++last;
			}

			vector(value_type&& val) : data() {
				data[0] = std::move(val);
				++last;
			}

			void push_back(const value_type& val) {
				if (last == data.end()) {
					data.expand_by(1);
				}

				*(last++) = val;
			}

			void push_back(value_type&& val) {
				if (last == data.end()) {
					data.expand_by(1);
				}

				*(last++) = std::move(val);
			}

			void push_front(const value_type& val) {
				if (last == data.end()) {
					data.expand_by(1);
				}

				shift_at(0);
				data[0] = val;
				++last;
			}

			void push_front(value_type&& val) {
				if (last == data.end()) {
					data.expand_by(1);
				}

				shift_at(0);
				data[0] = std::move(val);
				++last;
			}

			void insert(const value_type& val, size_t index = 0) {
				if (last == data.end()) {
					data.expand_by(1);
				}

				shift_at(index);
				data[index] = val;
				++last;
			}

			void insert(value_type&& val, size_t index = 0) {
				if (last == data.end()) {
					data.expand_by(1);
				}

				shift_at(index);
				data[index] = std::move(val);
				++last;
			}

			void shift_at(size_t index, long long amount = 1) {
				if (data[0] != value_type() && amount != 0) {
					if (amount > 0) {
						for (int i = last - begin(); i > (int)index; i--)
							data[i + amount] = std::move(data[i]);

						data[index + amount] = std::move(data[index]);
					}
					else {
						for (int i = index; i < last - begin(); i++)
							data[i] = std::move(data[i - amount]);
					}
				}
			}

			value_type& operator [] (size_t index) {
				return data[index];
			}

			const value_type& operator [] (size_t index) const {
				return data[index];
			}

			void erase_at(size_t index) {
				shift_at(index, -1);
				--last;
			}

			void erase(const value_type& val) {
				for (int i = 0; i <= last - begin(); i++) {
					if (data[i] == val) {
						shift_at(i, -1);
						--last;
						break;
					}
				}
			}

			void clear() noexcept {
				last = data.begin();
			}

			size_t size() const noexcept {
				return last - begin();
			}

			constexpr iterator begin() noexcept {
				return data.begin();
			}

			constexpr iterator begin() const noexcept {
				return data.begin();
			}

			constexpr reverse_iterator rend() noexcept {
				return data.rend();
			}

			constexpr reverse_iterator rend() const noexcept {
				return data.rend();
			}

			constexpr iterator end() noexcept {
				return last;
			}

			constexpr iterator end() const noexcept {
				return last;
			}

			constexpr reverse_iterator rbegin() noexcept {
				return static_cast<reverse_iterator>(last - 1);
			}

			constexpr reverse_iterator rbegin() const noexcept {
				return static_cast<reverse_iterator>(last - 1);
			}
	};
}