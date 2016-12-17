#pragma once
#include "BoyerMoore.h"

class BoyerMooreSunday: BoyerMoore
{
public:
	BoyerMooreSunday(const string& text, const string& pattern);
	vector<int>* Search() override;	
private:
	void Sunday();
};

inline BoyerMooreSunday::BoyerMooreSunday(const string& text, const string& pattern): BoyerMoore(text, pattern)
{
	badchar = vector<int>(NO_OF_CHARS, pattern.size());
}

inline vector<int>* BoyerMooreSunday::Search()
{
	vector<int>* occurences = new vector<int>();
	if (pattern.size() <= 0 || text.size() <= 0 || pattern.size()>text.size())
	{
		return occurences;
	}
	Sunday();
	int t = text.size();
	int p = pattern.size();
	int shift = 0; 
	while (shift <= (t - p)) {
		int i = p - 1;
		while (text[shift + i] == pattern[i] && i != 0)
		{
			--i;
		}

		if (i == 0) { 
			occurences->push_back(shift);
		}
		shift += badchar[text[shift + p]];
	}
	return occurences;
}

inline void BoyerMooreSunday::Sunday()
{
	for (int i = 0; i < pattern.size(); ++i)
	{
		badchar[pattern[i]] = pattern.size() - i;
	}
}
