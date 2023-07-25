/***********************************************************************************************************************
* Copyright © 2022-2023 Alex-1A
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

// v2
// Infinite Integer.cpp

#include "Infinite Integer.h"
#include <iomanip>
#include <sstream>

namespace infNums {
	// Public

	infInt::infInt(void) : sign(false) {} // Set to 0

	infInt::infInt(char const* str) {
		std::string copy = str;
		fromStr(copy);
	}

	infInt::infInt(std::string const& str) {
		std::string copy = str;
		fromStr(copy);
	}

	infInt::infInt(unsigned long long initial) : sign(false) {
		while (initial > 0) { // Add sizeof(uSmall) bytes at a time.
			inf.push_back(static_cast<uSmall>(initial));
			initial >>= 8 * sizeof(uSmall);
		}
	}

	infInt::infInt(long long initial) : sign(initial < 0) {
		unsigned long long init = (sign ? -initial : initial);
		while (init > 0) { // Make unsigned and add sizeof(uSmall) bytes at a time.
			inf.push_back(static_cast<uSmall>(init));
			init >>= 8 * sizeof(uSmall);
		}
	}


	infInt infInt::operator-(void) const {
		infInt copy(*this);
		copy.sign = !sign;
		return copy;
	}

	infInt const& infInt::negate(void) {
		sign = !sign;
		return *this;
	}

	infInt const& infInt::operator+=(infInt const& add) {
		if (add.inf.empty()) // Adding 0
			return *this;
		if (inf.empty()) // Adding to 0
			*this = add;
		else if (sign == add.sign) { // potitive + positive and negative + negative
			auto it1 = begin(inf);
			auto it2 = begin(add.inf);
			uSmall overflow = 0;

			do {
				uLarge check = *it1;											  // Check for
				check += *it2;													  // overflow
				check += overflow;												  // with larger
				*it1 += *it2 + overflow;										  // integer
				check == static_cast<uLarge>(*it1) ? overflow = 0 : overflow = 1; // type
				++it2; // Needs to be here in case of short circut.
			} while (++it1 != end(inf) && it2 != end(add.inf));

			if (it2 != end(add.inf)) { // First integer is smaller
				do {
					uLarge check = *it2;												// Check for
					check += overflow;													// overflow
					inf.push_back(*it2 + overflow);										// with larger
					it1 = end(inf);														// integer
					check == static_cast<uLarge>(*--it1) ? overflow = 0 : overflow = 1;	// type
				} while (++it2 != end(add.inf));
				++it1; // it1 = end(inf);
			}

			if (overflow) // Final overflow check
				if (it1 != end(inf)) {
					uLarge check = *it1;
					if (++check != static_cast<uLarge>(++*it1)) // Final overflow aplication overflow check.
						inf.push_back(1);
				}
				else
					inf.push_back(1);
		}
		else // potitive + negative and negative + positive
			*this -= -add;
		return *this;
	}

	infInt const& infInt::operator-=(infInt const& sub) {
		if (sub.inf.empty()) // Subtract 0
			return *this;
		if (inf.empty()) // Subtract from 0
			*this = -sub;
		else if (sign == sub.sign) { // potitive - positive and negative - negative
			auto it1 = begin(inf);
			auto it2 = begin(sub.inf);
			uSmall underflow = 0;

			do {
				uLarge check = *it1;												// Check for
				check -= *it2;														// underflow
				check -= underflow;													// with 
				*it1 -= *it2;														// larger
				*it1 -= underflow;													// integer
				check == static_cast<uLarge>(*it1) ? underflow = 0 : underflow = 1; // type
				++it2; // Needs to be here in case of short circut.
			} while (++it1 != end(inf) && it2 != end(sub.inf));

			if (it2 != end(sub.inf)) { // First integer is smaller
				do {
					inf.push_back(0 - *it2 - underflow);
					it1 = end(inf);
					underflow = 1; // Underflow garunteed
				} while (++it2 != end(sub.inf));
				it1 = end(inf);
			}

			if (underflow) { // Final underflow check
				bool flag = false; // 2's complement
				if (it1 != end(inf)) {
					uLarge check = *it1;
					if (--check != static_cast<uLarge>(--*it1))
						flag = true;
				}
				else
					flag = true;
				if (flag) {
					sign = !sign;
					for (auto it = begin(inf); it != end(inf); ++it)
						*it = ~*it;
					*this += 1uLL;
				}
			}

			compact();
		}
		else // potitive - negative and negative - positive
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
		if (inf.empty() || mult.inf.empty()) // *0
			return infInt();
		if (inf.size() == 1 && inf[0] == 1) // +-1 * x
			return sign ? -mult : mult;
		if (mult.inf.size() == 1 && mult.inf[0] == 1) // x * +-1
			return mult.sign ? -*this : *this;

		infInt result;
		size_t pos1 = 0;
		for (auto it1 : inf) { // For each section of this->inf...
			size_t pos2 = 0;
			for (auto it2 : mult.inf) { // ...for each section of mult.inf...
				uLarge temp = it1;
				temp *= it2; // ...multiply...
				infInt sum = temp;
				sum.shiftLeft(pos1 + pos2++); // ...shift left by number of positions the 2 source sections are...
				result += sum; // ...and add to result.
			}
			++pos1;
		}
		result.sign = sign != mult.sign; // Multiply signs.
		return result;
	}

	infInt const& infInt::operator*=(infInt const& mult) {
		*this = std::move(*this * mult);
		return *this;
	}

	infInt const& infInt::operator/=(infInt const& div) {
		if (div.inf.empty())
			throw std::domain_error("Attempted /0.");
		if (inf.empty())
			return *this;

		bool temp = sign != div.sign;
		if (div.inf.size() != 1 || div.inf[0] != 1) { // I don't know a fast way to divide.
			sign = div.sign; // Simplifies if below.
			infInt result;
			infInt const* op = (this == &div ? new infInt(div) : &div);
			if (op->sign) // -
				while (*this <= *op) { // Loop subtraction
					*this -= *op;
					++result;
				}
			else // +
				while (*this >= *op) { // Loop subtraction
					*this -= *op;
					++result;
				}
			if (this == &div)
				delete op;
			inf = std::move(result.inf);
		}
		sign = temp;
		return *this;
	}

	infInt infInt::operator/(infInt const& div)const {
		infInt result = *this;
		result /= div;
		return result;
	}

	infInt const& infInt::operator%=(infInt const& mod) {
		if (mod.inf.empty())
			throw std::domain_error("Attempted %0.");
		if (inf.empty())
			return *this;
		if (mod.inf.size() == 1 && mod.inf[0] == 1)
			*this = infInt();

		else { // I don't know a fast way to modulate.
			bool temp = sign;
			sign = mod.sign; // Simplifies if below.
			infInt const* op = (this == &mod ? new infInt(mod) : &mod);
			if (mod.sign) // -
				while (*this <= *op) // Loop subtraction
					*this -= *op;
			else // +
				while (*this >= *op) // Loop subtraction
					*this -= *op;
			if (this == &mod)
				delete op;
			sign = temp;
		}
		return *this;
	}

	infInt infInt::operator%(infInt const& mod)const {
		infInt result = *this;
		result %= mod;
		return result;
	}

	std::pair<infInt, infInt> infInt::divMod(infInt const& denom) const {
		if (denom.inf.empty())
			throw std::domain_error("Attempted /0.");
		if (inf.empty())
			return { infInt(),infInt() };
		if (denom.inf.size() == 1 && denom.inf[0] == 1)
			return { sign ? -*this : *this,infInt() };

		std::pair<infInt, infInt> result = { infInt(),*this };
		result.second.sign = denom.sign; // Simplifies if below.
		infInt const* op = (this == &denom ? new infInt(denom) : &denom);
		if (denom.sign) // -
			while (result.second <= *op) { // Loop subtraction
				result.second -= *op;
				++result.first;
			}
		else // +
			while (result.second >= *op) { // Loop subtraction
				result.second -= *op;
				++result.first;
			}
		if (this == &denom)
			delete op;
		result.first.sign = sign != denom.sign;
		result.second.sign = sign;
		return result;
	}


	infInt const& infInt::operator++(void) { // pre
		sign ? dec() : inc();
		return *this;
	}

	infInt infInt::operator++(int) { // post
		infInt result = *this;
		++*this;
		return result;
	}

	infInt const& infInt::operator--(void) { // pre
		sign ? inc() : dec();
		return *this;
	}

	infInt infInt::operator--(int) { // post
		infInt result = *this;
		--*this;
		return result;
	}


	bool infInt::operator!(void)const {
		return inf.empty();
	}

	bool infInt::operator==(infInt const& com)const {
		if (sign != com.sign || inf.size() != com.inf.size()) // Sign and size check.
			return false;
		auto it1 = begin(inf);
		auto it2 = begin(com.inf);
		while (it1 != end(inf)) // Check each segment.
			if (*it1++ != *it2++)
				return false;
		return true;
	}

	bool infInt::operator!=(infInt const& com)const {
		return !(*this == com);
	}

	bool infInt::operator<(infInt const& com)const {
		if (!sign && com.sign) // +<-
			return false;
		if (sign && !com.sign) // -<+
			return true;
		if (sign) // -<-
			return com.less(*this);
		return less(com); // +<+
	}

	bool infInt::operator>(infInt const& com)const {
		if (!sign && com.sign) // +>-
			return true;
		if (sign && !com.sign) // ->+
			return false;
		if (sign) // ->-
			return less(com);
		return com.less(*this); // +>+
	}

	bool infInt::operator<=(infInt const& com)const {
		if (!sign && com.sign) // +<=-
			return false;
		if (sign && !com.sign) // -<=+
			return true;
		if (sign) // -<=-
			return com.less(*this, true);
		return less(com, true); // +<=+
	}

	bool infInt::operator>=(infInt const& com)const {
		if (!sign && com.sign) // +>=-
			return true;
		if (sign && !com.sign) // ->=+
			return false;
		if (sign) // ->=-
			return less(com, true);
		return com.less(*this, true); // +>=+
	}


	infInt infInt::operator~(void)const {
		infInt result;
		for (auto it : inf) // Invert all
			result.inf.push_back(~it);
		result.compact(); // trim
		result.sign = (result.inf.empty() ? false : !sign); // Force sign false if 0, invert otherwise.
		return result;
	}

	infInt const& infInt::operator|=(infInt const& bits) {
		auto it1 = begin(inf);
		auto it2 = begin(bits.inf);
		while (it1 != end(inf) && it2 != end(bits.inf)) // OR all segments that overlap.
			*it1++ |= *it2++;
		while (it2 != end(bits.inf)) // Push all segments that "overflow".
			inf.push_back(*it2++);
		sign |= bits.sign;
		return *this;
	}

	infInt infInt::operator|(infInt const& bits)const {
		infInt result = *this;
		result |= bits;
		return result;
	}

	infInt const& infInt::operator&=(infInt const& bits) {
		while (inf.size() > bits.inf.size()) // Pop all segments that "overflow".
			inf.pop_back();
		auto it1 = begin(inf);
		auto it2 = begin(bits.inf);
		while (it1 != end(inf)) // AND all segments.
			*it1++ &= *it2++;
		if (inf.empty()) // Force sign false if result is 0.
			sign = false;
		else
			sign &= bits.sign;
		return *this;
	}

	infInt infInt::operator&(infInt const& bits)const {
		infInt result = *this;
		result &= bits;
		return result;
	}

	infInt const& infInt::operator^=(infInt const& bits) {
		auto it1 = begin(inf);
		auto it2 = begin(bits.inf);
		while (it1 != end(inf) && it2 != end(bits.inf)) // XOR all segments that overlap.
			*it1++ ^= *it2++;
		while (it2 != end(bits.inf)) // Push all segments that "overflow".
			inf.push_back(*it2++);
		sign ^= bits.sign;
		return *this;
	}

	infInt infInt::operator^(infInt const& bits)const {
		infInt result = *this;
		result ^= bits;
		return result;
	}

	infInt const& infInt::operator<<=(infInt bits) {
		if (bits.sign)
			throw std::domain_error("Attempted shift by negitive number.");
		if (inf.empty() || bits.inf.empty())
			return *this;

		size_t x = 0;
		while (bits >= 8u * sizeof(uSmall)) { // "Chunk" shift left and % bits.
			inf.push_front(0);
			bits -= 8u * sizeof(uSmall);
			++x; // Note where to start for later.
		}
		if (!bits.inf.empty()) { // If not done...
			uSmall carry = 0;
			while (x < inf.size()) { // ...for each not new segment...
				uSmall temp = inf[x] >> (8u * sizeof(uSmall) - bits.inf[0]); // ...save overflow...
				inf[x] <<= bits.inf[0]; // ...shift left...
				inf[x++] |= carry; // ...add carry...
				carry = temp; // ...and set carry to overflow,...
			}
			if (carry > 0) // ...then check for final carry.
				inf.push_back(carry);
		}
		return *this;
	}
	
	infInt infInt::operator<<(infInt const& bits)const {
		infInt result = *this;
		result <<= bits;
		return result;
	}
	
	infInt const& infInt::operator>>=(infInt bits) {
		if (bits.sign)
			throw std::domain_error("Attempted shift by negitive number.");
		if (inf.empty() || bits.inf.empty())
			return *this;

		while (bits >= 8u * sizeof(uSmall) && !inf.empty()) { // "Chunk" shift right and % bits.
			inf.pop_front();
			bits -= 8u * sizeof(uSmall);
		}
		if (bits.inf.size() && !inf.empty()) { // If not done...
			size_t x = 0;
			while (x < inf.size() - 1) { // ...for each segment execpt last...
				inf[x] >>= bits.inf[0]; // ...shift right...
				inf[x] |= inf[x + 1] << (8u * sizeof(uSmall) - bits.inf[0]); // ...and add carry from next segment,...
				++x;
			}
			inf[x] >>= bits.inf[0]; // ...then shift last segment right...
			if (inf[x] == 0) // ...and remove if set to 0.
				inf.pop_back();
		}
		if (inf.empty()) // Force sign false if 0.
			sign = false;
		return *this;
	}
	
	infInt infInt::operator>>(infInt const& bits)const {
		infInt result = *this;
		result >>= bits;
		return result;
	}


	infInt const& infInt::abs(void) {
		sign = false;
		return *this;
	}

	infInt abs(infInt const& value) {
		infInt result = value;
		result.sign = false;
		return result;
	}

	infInt pow(infInt const& base, infInt const& power) {
		infInt result = 1uLL;
		if (base == 1ull) // Base 1, result=1.
			return result;
		if (base == -1LL)
			return (power.inf.empty() ? 1uLL : (power.inf[0] % 2 ? -1LL : 1uLL)); // return (power % 2 ? -1 : 1); but O(1) instead of O(n^2).
		if (power < infInt())
			return infInt(); // -1<Result<1, truncates to 0.
		if (base == 2ull) // Base 2, just shift.
			result <<= power;
		else // Loop multiplication.
			for (infInt x; x < power; ++x)
				result *= base;
		return result;
	}

	infInt factorial(infInt const& value) {
		if (value < infInt())
			throw std::domain_error("Attempted factorial of negative number.");
		if (value <= 1uLL)
			return 1uLL;
		infInt result = value;
		infInt copy = value;
		while (copy > 2uLL) // Multiply all integers from 2 to value together.
			result *= --copy;
		return result;
	}

#if 0 // DEPRICATED
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
#endif

	infInt const& infInt::operator=(infInt const& other) { // Deep
		inf = other.inf;
		sign = other.sign;
		return *this;
	}

	infInt const& infInt::operator=(infInt&& other)noexcept { // Shallow
		inf = std::move(other.inf);
		sign = std::move(other.sign);
		return *this;
	}

	std::string infInt::str(bool leading0x)const {
		std::stringstream stream;
		if (sign)
			stream << '-';
		if (leading0x)
			stream << "0x";
		if (inf.empty())
			stream << '0';
		else
			for (auto it = rbegin(inf); it != rend(inf); ++it) // Print each segment to string.
				stream << std::hex << std::setw(sizeof(uSmall) * 2) << std::setfill('0') << *it;
		return stream.str();
	}

	size_t infInt::size(void) {
		return inf.size();
	}

	void infInt::shrink_to_fit(void) {
		inf.shrink_to_fit();
	}


	// Private

	void infInt::compact(void) {
		while (!inf.empty()) {
			if (inf[inf.size() - 1])
				return;
			inf.pop_back();
		}
	}

	void infInt::shiftLeft(size_t units) {
		while (units--)
			inf.push_front(0);
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
		while (it != end(inf)) // Increment until no overflow.
			if (++*it++ != 0)
				return;
		inf.push_back(1);
	}

	void infInt::dec(void) {
		if (inf.empty()) { // --0
			inf.push_back(1);
			sign = true;
			return;
		}
		if (*this == -1LL) { // ---1
			inf.clear();
			sign = false;
			return;
		}
		auto it = begin(inf); // --#, #!=0, #!=-1
		while (it != end(inf)) // Decrement until no underflow.
			if (--*it++ != USMALL_MAX) {
				compact();
				return;
			}
	}

	void infInt::fromStr(std::string& str) {
		if (str.find("0x") == 0)
			str.erase(0, 2);
		else if (str.find("-0x") == 0)
			str.erase(1, 2);
		auto tracker = 0;
		for (auto it = std::begin(str); it != end(str); ++it) { // Validate string
			*it = static_cast<char>(toupper(*it));
			if (*it == ' ' || (*it == '-' && it != begin(str)) || *it == '\t') {
				str.erase(it);
				it = begin(str) + tracker - 1;
			}
			else if ((*it > 'F' || (*it < 'A' && *it > '9') || *it < '0') && !(it == begin(str) && *it == '-'))
				throw std::range_error("Invalid string, should only contain characters '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', and 'F'.");
			else
				++tracker;
		}
		inf.clear();
		unsigned char count = 0;
		uSmall num = 0;
		auto pos = 0;
		for (auto it = end(str); --it != begin(str); ++count) { // Read string
			num = *it - (*it < 'A' ? '0' : ('0' + 7));
			if (count % 8 == 0)
				inf.push_back(num);
			else
				inf[pos] += num << 4 * (count % 8);
			if (count % 8 == 7)
				++pos;
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