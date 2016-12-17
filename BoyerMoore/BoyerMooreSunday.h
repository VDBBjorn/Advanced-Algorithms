#pragma once
#include "BoyerMoore.h"

class BoyerMooreSunday: BoyerMoore
{
public:
	BoyerMooreSunday(const string& text, const string& pattern);
	vector<int>* Search() override;	
};

inline BoyerMooreSunday::BoyerMooreSunday(const string& text, const string& pattern): BoyerMoore(text, pattern)
{
}

inline vector<int>* BoyerMooreSunday::Search()
{
}
