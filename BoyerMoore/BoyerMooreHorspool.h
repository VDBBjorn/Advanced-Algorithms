#pragma once
#include "BoyerMoore.h"

class BoyerMooreHorspool: BoyerMoore
{
public:
	BoyerMooreHorspool(const string& text, const string& pattern)
		: BoyerMoore(text, pattern)
	{
	}

	virtual ~BoyerMooreHorspool()
	{
	}
};
