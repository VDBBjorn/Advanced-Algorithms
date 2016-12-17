#pragma once
#include <string>
#include <vector>

using namespace std;

# define NO_OF_CHARS 256

/// <summary>
/// Compares text T and pattern P character per character on different starting points in the text 
/// </summary>
class BoyerMoore
{
public:
	BoyerMoore(string text, string pattern);

	virtual ~BoyerMoore()
	{
	}

	virtual vector<int>* Search() = 0;

protected:
	vector<int> badchar;
	string text;
	string pattern;
};

inline BoyerMoore::BoyerMoore(string text, string pattern): text(text), pattern(pattern)
{
}
