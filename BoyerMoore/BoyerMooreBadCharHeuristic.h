#pragma once
#include "BoyerMoore.h"
#include <vector>
#include <algorithm>

/// <summary>
/// Boyer Moore's simple bad character heuristic.
/// </summary>
class BoyerMooreBadCharHeuristic: protected BoyerMoore
{
public: 
	BoyerMooreBadCharHeuristic(string& text, string& pattern);
	vector<int>* Search() override;
protected:
	void BadCharHeuristic();
	vector<int> badchar;
};

inline BoyerMooreBadCharHeuristic::BoyerMooreBadCharHeuristic(string& text, string& pattern): BoyerMoore(text, pattern)
{
	badchar = vector<int>(NO_OF_CHARS, -1);
}

inline vector<int>* BoyerMooreBadCharHeuristic::Search()
{
	vector<int>* occurences = new vector<int>();
	if(pattern.size() <= 0 || text.size()<=0 || pattern.size()>text.size())
	{
		return occurences;
	}
	BadCharHeuristic();
	int t = text.size();
	int p = pattern.size();
	int shift = 0;
	while (shift <= (t - p))
	{
		int j = p - 1; // start at the end of the pattern
		while (j >= 0 && pattern[j] == text[shift + j])
			j--;

		if (j < 0)
		{
			occurences->push_back(shift);
			shift += (shift + p < t) ? p - badchar[text[shift + p]] : 1;
		}
		else
		{
			// ofwel 1 positie ofwel 0
			shift += max(1, j - badchar[text[shift + j]]);
		}
	}
	return occurences;
}

/// <summary>
/// Fill the actual value of last occurrence of a character
/// </summary>
inline void BoyerMooreBadCharHeuristic::BadCharHeuristic()
{
	for (int i = 0; i < pattern.size(); i++)
		badchar[static_cast<int>(pattern[i])] = i;
}
