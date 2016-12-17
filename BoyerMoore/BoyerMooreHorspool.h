#pragma once
#include "BoyerMoore.h"

class BoyerMooreHorspool: BoyerMoore
{
public:
	BoyerMooreHorspool(const string& text, const string& pattern);
	vector<int>* Search() override;
private:
	void Horspool();
	vector<int> badchar;
};

inline BoyerMooreHorspool::BoyerMooreHorspool(const string& text, const string& pattern): BoyerMoore(text, pattern)
{
	badchar = vector<int>(NO_OF_CHARS, pattern.size());
}

inline vector<int>* BoyerMooreHorspool::Search()
{
	vector<int>* occurences = new vector<int>();
	if (pattern.size() <= 0 || text.size() <= 0 || pattern.size()>text.size())
	{
		return occurences;
	}
	Horspool();
	int t = text.size();
	int p = pattern.size();
	int shift = 0;
	while (shift <= (t - p))
	{
		int i = p - 1;
		while (text[shift + i] == pattern[i] && i != 0)
		{
			--i;
		}

		if(i==0)
		{
			occurences->push_back(shift);
		}
		shift += badchar[text[shift + p - 1]];
	}
	return occurences;
}

inline void BoyerMooreHorspool::Horspool()
{
	for(int i = 0; i< pattern.size()-1 ; i++)
	{
		badchar[pattern[i]] = pattern.size() - i - 1;
	}
}
