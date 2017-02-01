#pragma once
#include "BoyerMoore.h"

class BoyerMooreGoodSuffix: BoyerMoore
{
public:
	BoyerMooreGoodSuffix(const string& text, const string& pattern);
	int Search() override;
private:
	void GoodSuffix();
	vector<int> goodSuffix;
};

inline BoyerMooreGoodSuffix::BoyerMooreGoodSuffix(const string& text, const string& pattern): BoyerMoore(text, pattern)
{
	goodSuffix = vector<int>(256, pattern.length());
}

inline int BoyerMooreGoodSuffix::Search()
{
	int patcount = pattern.length() - 1;
	int compcount = 0;
	int *no = 0;
	while (patcount < text.length())
	{
		int count = 0;
		while (count < pattern.length())
		{
			if (text[patcount - count] != pattern[pattern.length() - 1 - count])
			{
				compcount++;
				break;
			}
			else count++;
		}
		if (count == pattern.length())
		{
			(*no)++;
			*(pos + (*no) - 1) = (patcount - count + 1);
			patcount += pattern.length();
		}
		else
		{
			patcount += goodSuffix[1];
		}
	}
	return compcount;
}

inline void BoyerMooreGoodSuffix::GoodSuffix()
{
	for (int i = 0; i < pattern.length(); i++)
	{
		goodSuffix[pattern[i]] = pattern.length() - i - 1;
	}
}
