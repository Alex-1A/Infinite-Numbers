/***********************************************************************************************************************
* Copyright © 2022 Alex - 1A
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated
* documentation files (the "Software"), to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense, and\or sell copies of the Software,
* and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
* WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS
* OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
***********************************************************************************************************************/

// v1
// Infinite Integer.cpp

#include "Infinite Integer.h"
#include <iomanip>

// I plan to add comments throughout the .cpp file in the realitivly near future.

namespace infNums {
	// Public

	infInt::infInt(void) : sign(false) {
		inf.clear();
		compact();
	}

	infInt::infInt(char const* str) {
		std::string copy = str;
		fromStr(copy);
	}

	infInt::infInt(std::string const& str) {
		std::string copy = str;
		fromStr(copy);
	}

	template <typename type>
	infInt::infInt(type const& initial) : sign(initial < 0) {
		static_assert(std::is_integral_v<type>, "\"initial\" not of integral type.");
		inf.clear();

		float end = sizeof(initial);
		end /= sizeof(uSmall);
		for (auto x = 0; x < end; ++x) {
			uSmall expo = 8 * sizeof(uSmall) * x;
			uLarge power = 1;
			for (uSmall y = 0; y < expo; ++y)
				power *= 2;
			if (std::is_unsigned_v<type>) {
				uLarge value = initial;
				value /= power;
				inf.push_back(static_cast<uSmall>(value));
			}
			else {
				large value = std::abs(static_cast<large>(initial));
				value /= power;
				inf.push_back(static_cast<uSmall>(value));
			}
		}
		compact();
	}


	infInt infInt::operator-(void) const {
		infInt copy(*this);
		copy.sign = !sign;
		return copy;
	}

	infInt const& infInt::operator+=(infInt const& add) {
		if (add.inf.size() == 0)
			return *this;
		if (inf.size() == 0)
			*this = add;
		else if (sign == add.sign) {
			auto it1 = begin(inf);
			auto it2 = begin(add.inf);
			uSmall overflow = 0;

			do {
				uLarge check = *it1;
				check += *it2;
				check += overflow;
				*it1 += *it2 + overflow;
				check == static_cast<uLarge>(*it1) ? overflow = 0 : overflow = 1;
				++it2;
			} while (++it1 != end(inf) && it2 != end(add.inf));

			if (it2 != end(add.inf)) {
				do {
					uLarge check = *it2;
					check += overflow;
					inf.push_back(*it2 + overflow);
					it1 = end(inf);
					check == static_cast<uLarge>(*--it1) ? overflow = 0 : overflow = 1;
				} while (++it2 != end(add.inf));
				++it1;
			}

			if (overflow)
				if (it1 != end(inf)) {
					uLarge check = *it1;
					if (++check != static_cast<uLarge>(++ * it1))
						inf.push_back(1);
				}
				else
					inf.push_back(1);
		}
		else
			*this -= -add;
		return *this;
	}

	infInt const& infInt::operator-=(infInt const& sub) {
		if (sub.inf.size() == 0)
			return *this;
		if (inf.size() == 0)
			*this = -sub;
		else if (sign == sub.sign) {
			auto it1 = begin(inf);
			auto it2 = begin(sub.inf);
			uSmall underflow = 0;

			do {
				uLarge check = *it1;
				check -= *it2;
				check -= underflow;
				*it1 -= *it2;
				*it1 -= underflow;
				check == static_cast<uLarge>(*it1) ? underflow = 0 : underflow = 1;
				++it2;
			} while (++it1 != end(inf) && it2 != end(sub.inf));

			if (it2 != end(sub.inf)) {
				do {
					inf.push_back(0 - *it2 - underflow);
					it1 = end(inf);
					underflow = 1;
				} while (++it2 != end(sub.inf));
				it1 = end(inf);
			}

			if (underflow) {
				bool flag = false;
				if (it1 != end(inf)) {
					uLarge check = *it1;
					if (--check != static_cast<uLarge>(-- * it1))
						flag = true;
				}
				else
					flag = true;
				if (flag) {
					sign = !sign;
					for (auto it = begin(inf); it != end(inf); ++it)
						*it ^= 0xFFFFFFFF;
					*this += 1ULL;
				}
			}

			compact();
		}
		else
			*this += -sub;
		return *this;
	}

	infInt infInt::operator+(infInt const& add)const {
		infInt result = *this;
		result += add;
		return result;
	}

	infInt infInt::operator-(infInt const& sub)const {
		infInt result = *this;
		result -= sub;
		return result;
	}

	infInt infInt::operator*(infInt const& mult)const {
		if (inf.size() == 0 || mult.inf.size() == 0)
			return infInt(0);
		if (inf.size() == 1 && inf[0] == 1)
			if (sign)
				return -mult;
			else
				return mult;
		if (mult.inf.size() == 1 && mult.inf[0] == 1)
			if (mult.sign)
				return -*this;
			else
				return *this;

		infInt result = 0;
		size_t pos1 = 0;
		for (auto it1 : inf) {
			size_t pos2 = 0;
			for (auto it2 : mult.inf) {
				uLarge temp = it1;
				temp *= it2;
				infInt sum = temp;
				sum.shiftLeft(pos1 + pos2++);
				result += sum;
			}
			++pos1;
		}
		result.sign = sign != mult.sign;
		return result;
	}

	infInt const& infInt::operator*=(infInt const& mult) {
		*this = *this * mult;
		return *this;
	}

	infInt const& infInt::operator/=(infInt const& div) {
		if (div.inf.size() == 0)
			throw std::domain_error("Attempted /0.");
		if (inf.size() == 0)
			return *this;
		if (div.inf.size() == 1 && div.inf[0] == 1) {
			if (div.sign)
				*this = -*this;
		}

		else { // I don't know a fast way to divide.
			bool temp = sign != div.sign;
			sign = div.sign;
			infInt result = 0;
			if (div.sign)
				while (*this <= div) {
					*this -= div;
					result += 1ULL;
				}
			else
				while (*this >= div) {
					*this -= div;
					result += 1ULL;
				}
			*this = result;
			sign = temp;
		}
		return *this;
	}

	infInt infInt::operator/(infInt const& div)const {
		infInt result = *this;
		result /= div;
		return result;
	}

	infInt const& infInt::operator%=(infInt const& mod) {
		if (mod.inf.size() == 0)
			throw std::domain_error("Attempted %0.");
		if (inf.size() == 0)
			return *this;
		if (mod.inf.size() == 1 && mod.inf[0] == 1)
			*this = 0ULL;

		else { // I don't know a fast way to modulo.
			sign = mod.sign;
			if (mod.sign)
				while (*this <= mod)
					*this -= mod;
			else
				while (*this >= mod)
					*this -= mod;
			sign = false;
		}
		return *this;
	}

	infInt infInt::operator%(infInt const& mod)const {
		infInt result = *this;
		result %= mod;
		return result;
	}


	infInt const& infInt::operator++(void) {
		if (sign)
			dec();
		else
			inc();
		return *this;
	}

	infInt infInt::operator++(int) {
		infInt result = *this;
		++* this;
		return result;
	}

	infInt const& infInt::operator--(void) {
		if (sign)
			inc();
		else
			dec();
		return *this;
	}

	infInt infInt::operator--(int) {
		infInt result = *this;
		--* this;
		return result;
	}


	bool infInt::operator!(void)const {
		return !inf.size();
	}

	bool infInt::operator==(infInt const& com)const {
		if (sign != com.sign || inf.size() != com.inf.size())
			return false;
		auto it1 = begin(inf);
		auto it2 = begin(com.inf);
		while (it1 != end(inf))
			if (*it1++ != *it2++)
				return false;
		return true;
	}

	bool infInt::operator!=(infInt const& com)const {
		return !(*this == com);
	}

	bool infInt::operator<(infInt const& com)const {
		if (!sign && com.sign)
			return false;
		if (sign && !com.sign)
			return true;
		if (sign)
			return com.less(*this);
		return less(com);
	}

	bool infInt::operator>(infInt const& com)const {
		if (!sign && com.sign)
			return true;
		if (sign && !com.sign)
			return false;
		if (sign)
			return less(com);
		return com.less(*this);
	}

	bool infInt::operator<=(infInt const& com)const {
		if (!sign && com.sign)
			return false;
		if (sign && !com.sign)
			return true;
		if (sign)
			return com.less(*this, true);
		return less(com, true);
	}

	bool infInt::operator>=(infInt const& com)const {
		if (!sign && com.sign)
			return true;
		if (sign && !com.sign)
			return false;
		if (sign)
			return less(com, true);
		return com.less(*this, true);
	}


	infInt abs(infInt const& value) {
		infInt result = value;
		result.sign = false;
		return result;
	}

	infInt pow(infInt const& base, infInt const& power) {
		if (power.sign)
			return 0;
		infInt result = 1;
		for (infInt x = 0; x < power; ++x)
			result *= base;
		return result;
	}

	infInt factorial(infInt const& value) {
		if (value.sign)
			throw std::domain_error("Attempted factorial of negative number.");
		if (value.inf.size() == 0 || value <= 1)
			return 1;
		infInt result = value;
		infInt copy = value;
		while (copy > 2)
			result *= --copy;
		return result;
	}



	std::ostream& operator<<(std::ostream& stream, infInt const& out) {
		std::ios temp(nullptr);
		temp.copyfmt(stream);
		out.sign ? stream << "-0x" : stream << "0x";
		if (begin(out.inf) == end(out.inf))
			return stream << '0';
		for (auto it = rbegin(out.inf); it != rend(out.inf); ++it)
			stream << std::hex << std::setw(sizeof(uSmall) * 2) << std::setfill('0') << *it;
		stream.copyfmt(temp);
		return stream;
	}

	std::istream& operator>>(std::istream& stream, infInt& in) {
		std::string str;
		getline(stream, str);
		in.fromStr(str);
		return stream;
	}

	// Private

	void infInt::compact(void) {
		bool flag = true;
		while (flag && inf.size())
			if (inf[inf.size() - 1])
				flag = false;
			else
				inf.pop_back();
		inf.shrink_to_fit();
	}

	void infInt::shiftLeft(size_t units) {
		while (units--)
			inf.insert(begin(inf), 0);
	}

	bool infInt::less(infInt const& big, bool equal)const {
		if (inf.size() < big.inf.size())
			return true;
		if (inf.size() > big.inf.size())
			return false;
		auto it1 = rbegin(inf);
		auto it2 = rbegin(big.inf);
		while (it1 != rend(inf)) {
			if (*it1 < *it2)
				return true;
			else if (*it1 > *it2)
				return false;
			++it1;
			++it2;
		}
		return equal;
	}

	void infInt::inc(void) {
		auto it = begin(inf);
		while (it != end(inf))
			if (++ * it != 0)
				return;
		inf.push_back(1);
	}

	void infInt::dec(void) {
		if (*this == 0) {
			++* this;
			sign = true;
		}
		else {
			if (*this == -1)
				sign = false;
			bool flag = true;
			auto it = begin(inf);
			while (flag && it != end(inf))
				if (-- * it++ != USMALL_MAX)
					flag = false;
			compact();
		}
	}

	void infInt::fromStr(std::string& str) {
		if (str.find("0x") == 0)
			str.erase(0, 2);
		else if (str.find("-0x") == 0)
			str.erase(1, 2);
		auto tracker = 0;
		for (auto it = std::begin(str); it != end(str); ++it) {
			*it = static_cast<char>(toupper(*it));
			if (*it == ' ' || *it == '-' && it != begin(str) || *it == '\t') {
				str.erase(it);
				it = begin(str) + tracker - 1;
			}
			else if ((*it > 'F' || *it < 'A' && *it > '9' || *it < '0') && !(it == begin(str) && *it == '-'))
				throw std::range_error("Invalid string, should only contain characters '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', and 'F'.");
			else
				++tracker;
		}
		inf.clear();
		unsigned char count = 0;
		uSmall num = 0;
		auto pos = 0;
		for (auto it = end(str); --it != begin(str); ++count) {
			*it < 'A' ? num = *it - '0' : num = *it - ('0' + 7);
			if (count % 8 == 0)
				inf.push_back(num);
			else if (count % 8 == 7)
				inf[pos++] += num << 4 * 7;
			else
				inf[pos] += num << 4 * (count % 8);
		}
		if (str[0] == '-')
			sign = true;
		else {
			str[0] < 'A' ? num = str[0] - '0' : num = str[0] - ('0' + 7);
			if (count % 8 == 0)
				inf.push_back(num);
			else
				inf[pos] += num << 4 * (count % 8);
			sign = false;
		}
		compact();
	}
}