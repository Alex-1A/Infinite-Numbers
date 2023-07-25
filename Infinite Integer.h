#pragma once

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
// Infinite Integer.h

#include <deque>
#include <string>
#include <utility> // std::pair

namespace infNums {
	typedef unsigned long long uLarge;	// Quick change point, in case you want more
	typedef int small;					// than a 64 exbibyte (sizeof(uSmall) * 2
	typedef unsigned int uSmall;		// ^ (SIZE_MAX + 1) + 1) integer.  Large
	typedef long long large;			// needs to be at least double the size of
	#define USMALL_MAX UINT_MAX			// small, and both need to be integral types.
	static_assert(sizeof(small) * 2 <= sizeof(large) && sizeof(small) == sizeof(uSmall) && sizeof(small) == sizeof(uSmall), "Inconsistent sizes");

	class infInt {
		public:
			// Constuctors

			/*******************************************************************************
			* Default constructor
			*
			* Takes void.
			*
			* Sets value to 0.
			* 
			* Best:  O(1)
			* Worst:  O(1)
			*******************************************************************************/
			infInt(void);

			/*******************************************************************************
			* String constructor
			*
			* Takes hexadecimal string.
			*
			* Sets value to value string represents.
			* 
			* Best:  O(n)  n=length of str
			* Worst:  O(n)  n=length of str
			* 
			* Note:  Assumes null terminated.
			*******************************************************************************/
			infInt(char const* str);

			/*******************************************************************************
			* std::string constructor
			*
			* Takes hexadecimal string.
			*
			* Sets value to value string represents.
			* 
			* Best:  O(n)  n=str.length()
			* Worst:  O(n)  n=str.length()
			*******************************************************************************/
			infInt(std::string const& str);

			/*******************************************************************************
			* unsigned long long constructor
			*
			* Takes an unsigned long long.
			*
			* Sets value to initial.
			* 
			* Best:  O(1)
			* Worst:  O(1)?  O(sizeof(initial)/sizeof(uSmall))
			*******************************************************************************/
			infInt(unsigned long long initial);

			/*******************************************************************************
			* long long constructor
			*
			* Takes a long long.
			*
			* Sets value to initial.
			* 
			* Best:  O(1)
			* Worst:  O(1)?  O(sizeof(initial)/sizeof(uSmall))
			*******************************************************************************/
			infInt(long long initial);

			/*******************************************************************************
			* Copy constructor
			*
			* Takes an infInf const&.
			*
			* Sets value to given value.
			* 
			* Best:  O(n)  n=this->size
			* Worst:  O(n)  n=this->size
			*******************************************************************************/
			infInt(infInt const&) = default;

			/*******************************************************************************
			* Destructive copy constructor
			*
			* Takes an infInf const&.
			*
			* Sets value to given value.
			*
			* Best:  O(1)
			* Worst:  O(1)
			*******************************************************************************/
			infInt(infInt&&)noexcept = default;

			/*******************************************************************************
			* Destructor
			* 
			* Best:  O(n)  n=this->size()
			* Worst:  O(n)  n=this->size()
			*******************************************************************************/
			~infInt(void) = default;


			// Basic math operators

			/*******************************************************************************
			* Operator - (unary)
			*
			* Takes self.
			*
			* Returns negated value.
			* 
			* Best:  O(n)  n=this->size()
			* Worst:  O(n)  n=this->size()
			*******************************************************************************/
			infInt operator-(void) const;

			/*******************************************************************************
			* negate
			*
			* Returns const& to self.
			* 
			* x = -x; without the copy.
			* 
			* Best:  O(1)
			* Worst:  O(1)
			*******************************************************************************/
			infInt const& negate(void);

			/*******************************************************************************
			* Operator +=
			*
			* Takes self, value to add.
			*
			* Returns const& to self.
			* 
			* Best:  O(1)
			* Worst:  O(n)  n=larger of this->size() and add.size()
			*******************************************************************************/
			infInt const& operator+=(infInt const& add);

			/*******************************************************************************
			* Operator -=
			*
			* Takes self, value to subtract.
			*
			* Resturns const& to self.
			* 
			* Best:  O(1)
			* Worst:  O(n)  n=larger of this->size() and sub.size()
			*******************************************************************************/
			infInt const& operator-=(infInt const& sub);

			/*******************************************************************************
			* Operator +
			*
			* Takes self, value to add.
			*
			* Returns copy of added numbers.
			* 
			* Best:  O(n)  n=this->size()
			* Worst:  O(n)  n=this->size() and larger of this->size() and add.size()
			*******************************************************************************/
			infInt operator+(infInt const& add) const;

			/*******************************************************************************
			* Operator - (binary)
			*
			* Takes self, value to subtract.
			*
			* Returns copy of subtracted numbers.
			* 
			* Best:  O(n)  n=this->size()
			* Worst:  O(n)  n=this->size() and larger of this->size() and sub.size()
			*******************************************************************************/
			infInt operator-(infInt const& sub) const;

			/*******************************************************************************
			* Operator *
			*
			* Takes self, value to multiply.
			*
			* Returns copy of multiplied numbers.
			* 
			* Best:  O(1)
			* Worst:  O(n^3)  n^3=this->size()*mult.size()*O(+=)
			* Note:  While looping addition would be O(n^2), it would only become faster at
			*		 this->size()+mult.size()~2^84 in my testing, at which point you'd be
			*		 far out of RAM addresses on a 64-bit computer, let alone actual RAM.
			*		 N would be correlated with the numbers themselves, instead
			*		 of the size of the numbers as seen here with the hardware
			*		 multiplication followed by summing the multiplications.
			*******************************************************************************/
			infInt operator*(infInt const& mult) const;

			/*******************************************************************************
			* Operator *=
			*
			* Takes self, value to multiply.
			*
			* Returns const& to self.
			* 
			* Best:  O(n)  n=this->size()
			* Worst:  O(n^3)  n^3=this->size()*mult.size()*O(+=)
			* Note:  While looping addition would be O(n^2), it would only become faster at
			*		 this->size()+mult.size()~2^84 in my testing, at which point you'd be
			*		 far out of RAM addresses on a 64-bit computer, let alone actual RAM. N
			*		 would be correlated with the numbers themselves, instead of the size of
			*		 the numbers as seen here with the hardware multiplication followed by
			*		 summing the multiplications.  Both also require a new infInt or copy.
			*******************************************************************************/
			infInt const& operator*=(infInt const& mult);

			/*******************************************************************************
			* Operator /=
			*
			* Takes self, value to divide.
			*
			* Returns const& to self.
			* 
			* Best:  O(1)
			* Worst:  O(n^2)  n^2=M*N  M=*this/div  N=larger of this->size() and div.size().
			*******************************************************************************/
			infInt const& operator/=(infInt const& div);

			/*******************************************************************************
			* Operator /
			*
			* Takes self, value to divide.
			*
			* Returns copy of divided numbers.
			* 
			* Best:  O(this->size())
			* Worst:  O(n^2)  n^2=M*N  M=*this/div  N=larger of this->size() and div.size().
			*******************************************************************************/
			infInt operator/(infInt const& div) const;

			/*******************************************************************************
			* Operator %=
			*
			* Takes self, value to modulate.
			*
			* Returns const& to self.
			* 
			* Best:  O(1)
			* Worst:  O(n^2)  n^2=M*N  M=*this/mod  N=larger of this->size() and mod.size().
			*******************************************************************************/
			infInt const& operator%=(infInt const& mod);

			/*******************************************************************************
			* Operator %
			*
			* Takes self, value to modulate.
			*
			* Returns copy of modulated numbers.
			* 
			* Best:  O(this->size())
			* Worst:  O(n^2)  n^2=M*N  M=*this/div  N=larger of this->size() and div.size().
			*******************************************************************************/
			infInt operator%(infInt const& mod) const;

			/*******************************************************************************
			* Division and modulus
			*
			* Takes self, value to divide and modulate.
			*
			* Returns division and modulus results in a pair.
			* 
			* Best:  O(1)
			* Worst:  O(n^2)  n^2=M*N  M=*this/denom
			*						   N=larger of this->size() and denom.size().
			*******************************************************************************/
			std::pair<infInt, infInt> divMod(infInt const& denom) const;

			//Increment and Decrement

			/*******************************************************************************
			* Operator ++ (pre)
			*
			* Takes self.
			*
			* Returns const& to self.
			*
			* Increments value.
			* 
			* Best:  O(1)
			* Worst:  O(n)  n=this->size()
			* 
			* Note:  O(1) for the vast majority of numbers, specifically
					 (2^(sizeof(uSmall)*8)-1)/2^(sizeof(uSmall)*8) of numbers.
			*******************************************************************************/
			infInt const& operator++(void);

			/*******************************************************************************
			* Operator ++ (post)
			*
			* Takes self.
			*
			* Returns copy of self.
			*
			* Increments value.
			* 
			* Best:  O(n)  n=this->size()
			* Worst:  O(n)  n=this->size()
			*******************************************************************************/
			infInt operator++(int);

			/*******************************************************************************
			* Operator -- (pre)
			*
			* Takes self.
			*
			* Returns const& to self.
			*
			* Decrements value.
			* 
			* Best:  O(1)
			* Worst:  O(n)  n=this->size()
			* 
			* Note:  O(1) for the vast majority of numbers, specifically
					 (2^(sizeof(uSmall)*8)-1)/2^(sizeof(uSmall)*8) of numbers.
			*******************************************************************************/
			infInt const& operator--(void);

			/*******************************************************************************
			* Operator -- (post)
			*
			* Takes self.
			*
			* Returns copy of self.
			*
			* Decrements value.
			* 
			* Best:  O(n)  n=this->size()
			* Worst:  O(n)  n=this->size()
			*******************************************************************************/
			infInt operator--(int);

			// Comparitors

			/*******************************************************************************
			* Operator !
			*
			* Takes self.
			*
			* Returns inverted bool of self, AKA returns if 0.
			* 
			* Best:  O(1)
			* Worst:  O(1)
			*******************************************************************************/
			bool operator!(void)const;

			/*******************************************************************************
			* Operator ==
			*
			* Takes self, value to compare.
			*
			* Returns whether values are equal.
			* 
			* Best:  O(1)
			* Worst:  O(n)  n=this->size()
			*******************************************************************************/
			bool operator==(infInt const& com)const;

			/*******************************************************************************
			* Operator !=
			*
			* Takes self, value to compare.
			*
			* Returns whether values aren't equal.
			* 
			* Best:  O(1)
			* Worst:  O(n)  n=this->size()
			*******************************************************************************/
			bool operator!=(infInt const& com)const;

			/*******************************************************************************
			* Operator <
			*
			* Takes self, value to compare.
			*
			* Returns whether self < compare value.
			* 
			* Best:  O(1)
			* Worst:  O(n)  n=this->size()
			*******************************************************************************/
			bool operator<(infInt const& com)const;

			/*******************************************************************************
			* Operator >
			*
			* Takes self, value to compare.
			*
			* Returns whether self > compare value.
			* 
			* Best:  O(1)
			* Worst:  O(n)  n=this->size()
			*******************************************************************************/
			bool operator>(infInt const& com)const;

			/*******************************************************************************
			* Operator <=
			*
			* Takes self, value to compare.
			*
			* Returns whether self <= compare value.
			* 
			* Best:  O(1)
			* Worst:  O(n)  n=this->size()
			*******************************************************************************/
			bool operator<=(infInt const& com)const;

			/*******************************************************************************
			* Operator >=
			*
			* Takes self, value to compare.
			*
			* Returns whether self >= compare value.
			* 
			* Best:  O(1)
			* Worst:  O(n)  n=this->size()
			*******************************************************************************/
			bool operator>=(infInt const& com)const;

			// Bitwise operators

			/*******************************************************************************
			* Operator ~
			*
			* Takes self.
			*
			* Returns bitwise negated value.
			* 
			* Best:  O(n)  n=this->size()
			* Worst:  O(n)  n=this->size()
			*
			* NOTE:  Not standard result of '~', result will be 0xf...f for
			* however many bytes are currently in use minus the number negated,
			* with the opposite sign.  Bytes in use = size() * sizeof(uSmall).
			*******************************************************************************/
			infInt operator~(void)const;

			/*******************************************************************************
			* Operator |=
			*
			* Takes self, bits to OR with.
			*
			* Returns const& to self.
			* 
			* Best:  O(n)  n=bits.size()
			* Worst:  O(n)  n=bits.size()
			*******************************************************************************/
			infInt const& operator|=(infInt const& bits);

			/*******************************************************************************
			* Operator |
			*
			* Takes self, bits to OR with.
			*
			* Returns copy of ORed value.
			* 
			* Best:  O(n)  n=larger of this->size() and bits.size()
			* Worst:  O(n)  n=larger of this->size() and bits.size()
			*******************************************************************************/
			infInt operator|(infInt const& bits)const;

			/*******************************************************************************
			* Operator &=
			*
			* Takes self, bits to AND with.
			*
			* Returns const& to self.
			* 
			* Best:  O(n)  n=this->size()
			* Worst:  O(n)  n=this->size()
			*******************************************************************************/
			infInt const& operator&=(infInt const& bits);

			/*******************************************************************************
			* Operator &
			*
			* Takes self, bits to AND with.
			*
			* Returns copy of ANDed value.
			* 
			* Best:  O(n)  n=larger of this->size() and bits.size()
			* Worst:  O(n)  n=larger of this->size() and bits.size()
			*******************************************************************************/
			infInt operator&(infInt const& bits)const;

			/*******************************************************************************
			* Operator ^=
			*
			* Takes self, bits to XOR with.
			*
			* Returns const& to self.
			* 
			* Best:  O(n)  n=bits.size()
			* Worst:  O(n)  n=bits.size()
			*******************************************************************************/
			infInt const& operator^=(infInt const& bits);

			/*******************************************************************************
			* Operator ^
			*
			* Takes self, bits to XOR with.
			*
			* Returns copy of XORed value.
			* 
			* Best:  O(n)  n=larger of this->size() and bits.size()
			* Worst:  O(n)  n=larger of this->size() and bits.size()
			*******************************************************************************/
			infInt operator^(infInt const& bits)const;
			
			/*******************************************************************************
			* Operator <<=
			*
			* Takes self, number of bits to shift.
			*
			* Returns const& to self.
			* 
			* Best:  O(n)  n=bits/(8*sizeof(uSmall))+bits/(8%sizeof(uSmall))
			* Worst:  O(n)   n=larger of this->size() and
										bits/(8*sizeof(uSmall))+bits/(8%sizeof(uSmall))
			*
			* NOTE:  Magitude logical shifted, sign not touched.
			*******************************************************************************/
			infInt const& operator<<=(infInt bits);

			/*******************************************************************************
			* Operator <<
			*
			* Takes self, number of bits to shift.
			*
			* Returns copy of shifted value.
			* 
			* Best:  O(n)  n=larger of copy of this->size() and
									   bits/(8*sizeof(uSmall))+bits/(8%sizeof(uSmall))
			* Worst:  O(n)  n=larger of copy of this->size() and
										bits/(8*sizeof(uSmall))+bits/(8%sizeof(uSmall))
			*
			* NOTE:  Magitude logical shifted, sign not touched.
			*******************************************************************************/
			infInt operator<<(infInt const& bits)const;

			/*******************************************************************************
			* Operator >>=
			*
			* Takes self, number of bits to shift.
			*
			* Returns const& to self.
			* 
			* Best:  O(n)  n=bits/(8*sizeof(uSmall))+bits/(8%sizeof(uSmall))
			* Worst:  O(n)  n=larger of this->size() and
										bits/(8*sizeof(uSmall))+bits/(8%sizeof(uSmall))
			*
			* NOTE:  Magitude logical shifted, sign not touched.
			*******************************************************************************/
			infInt const& operator>>=(infInt bits);

			/*******************************************************************************
			* Operator >>
			*
			* Takes self, number of bits to shift.
			*
			* Returns copy of shifted value.
			* 
			* Best:  O(n)  n=larger of copy of this->size() and
									   bits/(8*sizeof(uSmall))+bits/(8%sizeof(uSmall))
			* Worst:  O(n)  n=larger of copy of this->size() and
										bits/(8*sizeof(uSmall))+bits/(8%sizeof(uSmall))
			*
			* NOTE:  Magitude logical shifted, sign not touched.
			*******************************************************************************/
			infInt operator>>(infInt const& bits)const;

			// Other math

			/*******************************************************************************
			* Absolute value
			*
			* Returns const& to self.
			* 
			* x = abs(x); without the copy.
			* 
			* Best:  O(1)
			* Worst:  O(1)
			*******************************************************************************/
			infInt const& abs(void);
			
			/*******************************************************************************
			* Absolute value
			*
			* Takes a value to make positive.
			*
			* Returns copy of positive value.
			* 
			* Best:  O(n)  n=value.size()
			* Worst:  O(n)  n=value.size()
			*******************************************************************************/
			friend infInt abs(infInt const& value);

			/*******************************************************************************
			* Power
			*
			* Takes a base and power.
			*
			* Returns base to the power.
			*
			* Best:  O(n)  n=power/(8*sizeof(uSmall)+power/(8%sizeof(uSmall)
			* Worst:  O(n^3)  n^3=M*N^2  M=power  N=base.size()
			*******************************************************************************/
			friend infInt pow(infInt const& base, infInt const& power);

			// Factorial under class

			// Streams - DEPRICATED
#if 0
			/*******************************************************************************
			* Operator <<
			*
			* Takes ostream, value to print to stream.
			*
			* Reterns stream.
			*******************************************************************************/
			friend std::ostream& operator<<(std::ostream& stream, infInt const& out);

			/*******************************************************************************
			* Operator >>
			*
			* Takes ostream, location to put value read from stream.
			*
			* Returns stream.
			*******************************************************************************/
			friend std::istream& operator>>(std::istream& stream, infInt& in);
#endif
			// Other

			/*******************************************************************************
			* operator =
			*
			* Takes value.
			*
			* Returns const& to self.
			*
			* Best:  O(n)  n=this->size()+other.size()
			* Worst:  O(n)  n=this->size()+other.size()
			*******************************************************************************/
			infInt const& operator=(infInt const& other);

			/*******************************************************************************
			* Destructive operator =
			*
			* Takes value.
			*
			* Returns const& to self.
			*
			* Best:  O(n)  n=this->size()
			* Worst:  O(n)  n=this->size()+other.size()
			*******************************************************************************/
			infInt const& operator=(infInt&& other)noexcept;

			/*******************************************************************************
			* str
			*
			* Takes whether to put "0x" in the string.
			*
			* Returns copy of value as hexadecimal string.
			* 
			* Best:  O(n)  n=this->size()
			* Worst:  O(n)  n=this->size()
			*******************************************************************************/
			std::string str(bool leading0x = true)const;

			/*******************************************************************************
			* size
			*
			* Returns size of internal std::deque.
			* 
			* Best:  O(1)
			* Worst:  O(1)
			*******************************************************************************/
			size_t size(void);

			/*******************************************************************************
			* shrink_to_fit
			*
			* Trims unused memory.
			* 
			* Best:  O(1)?  Whatever std::deque::shrink_to_fit best O() is.
			* Worst:  O(n)  n=this->size()
			*******************************************************************************/
			void shrink_to_fit(void);

		private:
			std::deque<uSmall> inf; // The number
			bool sign; // Whether the number is negative.

			void compact(void); // Trims leading 0s, leading 0s will break it.  Best:  O(this->size())  Worst:  O(this->size())
			void shiftLeft(size_t units); // shift left by "units" sizeof(uSmall).  Best:  O(units)  Worst:  O(units)
			bool less(infInt const& big, bool equal = false)const; // Compare magnutudes, true if *this<big, false if *this>big, "equal" if *this==big.  Best:  O(1)  Worst:  O(this->size())
			void inc(void); // Increment magnitude  Best:  O(1)  Worst:  O(this->size())
			void dec(void); // Decrement magnitude  Best:  O(1)  Worst:  O(this->size())
			void fromStr(std::string& str); // Make from string.  Best:  O(str.length())  Worst:  O(str.length())
	};

	/*******************************************************************************
	* Factorial
	*
	* Takes value to explode.
	*
	* Returns copy of exploded value.
	*
	* Best:  O(n^3)  n=value
	* Worst:  O(n^3)  n=value
	*******************************************************************************/
	infInt factorial(infInt const& value);
}