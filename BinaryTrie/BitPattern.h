#ifndef __BITPATROON
#define __BITPATROON
#include <iostream>
using std::ostream;
typedef unsigned int uint;
const int patternLenght = sizeof(uint) * 3;

class BitPattern
{
public:
	uint bits;

	explicit BitPattern(uint u = 0) : bits(u)
	{
	};
	//EN-operator
	BitPattern operator&(const BitPattern& b) const
	{
		return BitPattern(bits & b.bits);
	}

	//OF-operator
	BitPattern operator|(const BitPattern& b) const
	{
		return BitPattern(bits | b.bits);
	}

	//EN-=-operator
	const BitPattern& operator&=(const BitPattern& b)
	{
		bits &= b.bits;
		return *this;
	}

	//OF-=-operator
	const BitPattern& operator|=(const BitPattern& b)
	{
		bits |= b.bits;
		return *this;
	}

	//logische operatoren
	bool en(const BitPattern& b) const
	{
		return (bits & b.bits) != 0;
	}

	bool of(const BitPattern& b) const
	{
		return (bits | b.bits) != 0;
	}

	//let op: shiftoperaties verplaatsen niets als verplaatsing >= patroonlengte.
	BitPattern shiftlinks(uint verplaatsing) const
	{
		return BitPattern(bits << verplaatsing);
	}

	BitPattern shiftrechts(uint verplaatsing) const
	{
		return BitPattern(bits >> verplaatsing);
	}

	//niet beveiligd: u moet kleiner dan patroonlengte zijn.
	static BitPattern eenbit(uint u)
	{
		return BitPattern(uint(1) << (patternLenght - 1 - u));
	}

	bool operator==(const BitPattern& b) const
	{
		for (int i = 0; i < patternLenght; i++)
		{
			if (b.en(eenbit(i)) != this->en(eenbit(i)))
			{
				return false;
			}
		}
		return true;
	}

	friend ostream& operator<<(ostream& os, const BitPattern& b)
	{
		for (int i = 0; i < patternLenght; i++)
		{
			os << b.en(eenbit(i));
		}
		return os;
	}
};
#endif
