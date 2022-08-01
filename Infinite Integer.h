#pragma once

/***********************************************************************************************************************
* Copyright © 2022 Alex-1A
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
// Infinite Integer.h

#include <vector>
#include <iostream>
#include <string>

#ifndef infNums
namespace infNums {
	#define USMALL_MAX UINT_MAX			// Quick change point, in case you want
	typedef int small;					// more than a 64 exbibyte (sizeof
	typedef unsigned int uSmall;		// (uSmall) * 2 ^(SIZE_MAX + 1) + 1)
	typedef long long large;			// integer.  Large needs to be at least
	typedef unsigned long long uLarge;	// double the size of small, and both
										// need to be integral types.
	class infInt {
		public:
			// Constuctors

			/*******************************************************************************
			* Default constructor
			*
			* Takes void.
			*
			* Sets value to 0.
			*******************************************************************************/
			infInt(void);

			/*******************************************************************************
			* String constructor
			*
			* Takes hexadecimal string.
			*
			* Sets value to value string represents.
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
			*******************************************************************************/
			infInt(std::string const& str);

			/*******************************************************************************
			* Template constructor
			*
			* Takes integral types.
			*
			* Sets value to initial.
			*******************************************************************************/
			template <typename type>
			infInt(type const& initial);


			// Basic math operators

			/*******************************************************************************
			* Operator - (unary)
			*
			* Takes self.
			*
			* Returns negated infInt.
			*******************************************************************************/
			infInt operator-(void) const;

			/*******************************************************************************
			* Operator +=
			*
			* Takes self, value to add.
			*
			* Returns const& to self.
			*******************************************************************************/
			infInt const& operator+=(infInt const& add);

			/*******************************************************************************
			* Operator -=
			*
			* Takes self, value to subtract.
			*
			* Resturns const& to self.
			*******************************************************************************/
			infInt const& operator-=(infInt const& sub);

			/*******************************************************************************
			* Operator +
			*
			* Takes self, value to add.
			*
			* Returns copy of added numbers.
			*******************************************************************************/
			infInt operator+(infInt const& add) const;

			/*******************************************************************************
			* Operator - (binary)
			*
			* Takes self, value to subtract.
			*
			* Returns copy of subtracted numbers.
			*******************************************************************************/
			infInt operator-(infInt const& sub) const;

			/*******************************************************************************
			* Operator *
			*
			* Takes self, value to multiply.
			*
			* Returns copy of multiplied numbers.
			*
			* Note:  X*Y may preform slightly differently than Y*X.
			*******************************************************************************/
			infInt operator*(infInt const& mult) const;

			/*******************************************************************************
			* Operator *=
			*
			* Takes self, value to multiply.
			*
			* Returns const& to self.
			*
			* Note:  *= isn't faster than *.
			*******************************************************************************/
			infInt const& operator*=(infInt const& mult);

			/*******************************************************************************
			* Operator /=
			*
			* Takes self, value to divide.
			*
			* Returns const& to self.
			*
			* Note:  Very slow, fast way to divide by certin powers of 2 planned for future.
			*******************************************************************************/
			infInt const& operator/=(infInt const& div);

			/*******************************************************************************
			* Operator /
			*
			* Takes self, value to divide.
			*
			* Returns copy of divided numbers.
			*
			* Note:  Very slow, fast way to divide by certin powers of 2 planned for future.
			*******************************************************************************/
			infInt operator/(infInt const& div) const;

			/*******************************************************************************
			* Operator %=
			*
			* Takes self, value to modulo.
			*
			* Returns const& to self.
			*
			* Note:  Very slow, fast way to modulo by certin powers of 2 planned for future.
			*******************************************************************************/
			infInt const& operator%=(infInt const& mod);

			/*******************************************************************************
			* Operator %
			*
			* Takes self, value to modulo.
			*
			* Returns copy of moduloed numbers.
			*
			* Note:  Very slow, fast way to modulo by certin powers of 2 planned for future.
			*******************************************************************************/
			infInt operator%(infInt const& mod) const;

			//Increment and Decrement

			/*******************************************************************************
			* Operator ++ (pre)
			*
			* Takes self.
			*
			* Returns const& to self.
			*
			* Increments value.
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
			*******************************************************************************/
			infInt operator--(int);

			// Comparitors

			/*******************************************************************************
			* Operator !
			*
			* Takes self.
			*
			* Returns negated bool self.
			*******************************************************************************/
			bool operator!(void)const;

			/*******************************************************************************
			* Operator ==
			*
			* Takes self, value to compare.
			*
			* Returns whether values are equal.
			*******************************************************************************/
			bool operator==(infInt const& com)const;

			/*******************************************************************************
			* Operator !=
			*
			* Takes self, value to compare.
			*
			* Returns whether values aren't equal.
			*******************************************************************************/
			bool operator!=(infInt const& com)const;

			/*******************************************************************************
			* Operator <
			*
			* Takes self, value to compare.
			*
			* Returns whether self < compare value.
			*******************************************************************************/
			bool operator<(infInt const& com)const;

			/*******************************************************************************
			* Operator >
			*
			* Takes self, value to compare.
			*
			* Returns whether self > compare value.
			*******************************************************************************/
			bool operator>(infInt const& com)const;

			/*******************************************************************************
			* Operator <=
			*
			* Takes self, value to compare.
			*
			* Returns whether self <= compare value.
			*******************************************************************************/
			bool operator<=(infInt const& com)const;

			/*******************************************************************************
			* Operator >=
			*
			* Takes self, value to compare.
			*
			* Returns whether self >= compare value.
			*******************************************************************************/
			bool operator>=(infInt const& com)const;

			// Other math

			/*******************************************************************************
			* Absolute value
			*
			* Takes a value to make positive.
			*
			* Returns copy of positive value.
			*******************************************************************************/
			friend infInt abs(infInt const& value);

			/*******************************************************************************
			* Power
			*
			* Takes a base and power.
			*
			* Returns base to the power.
			*******************************************************************************/
			friend infInt pow(infInt const& base, infInt const& power);

			/*******************************************************************************
			* Factorial
			*
			* Takes value to explode.
			*
			* Returns copy of exploded value.
			*******************************************************************************/
			friend infInt factorial(infInt const& value);

			// Streams

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

		private:
			std::vector<uSmall> inf;
			bool sign;

			void compact(void);
			void shiftLeft(size_t units);
			bool less(infInt const& big, bool equal = false)const;
			void inc(void);
			void dec(void);
			void fromStr(std::string& str);
	};
}

#elif
static_assert(false, "Check your includes and\\or defines, infNums is already defined.");
#endif
