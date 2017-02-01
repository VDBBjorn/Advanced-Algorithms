#pragma once
#include <string>
#include <vector>

using namespace std;

class KarpRabin
{
public:
	KarpRabin(string text, string pattern) : text(text), pattern(pattern)
	{
	}

	vector<int>& Search();
protected:
	string text, pattern;
	int d = 256;
	int r = 101;
};

inline vector<int>& KarpRabin::Search()
{
	vector<int>* found = new vector<int>();
	int p = pattern.size();
	int t = text.size();

	int p0 = 0;
	int t0 = 0;

	int h = 1;
	for (int i = 0; i < pattern.size() - 1; i++)
	{
		h = (h * d) % r;
	}
	for (int i = 0; i < p; i++)
	{
		p0 = (d * p0 + pattern[i]) % r;
		t0 = (d * t0 + text[i]) % r;
	}

	int tj = t0;

	for (int i = 0; i <= t - p; i++)
	{
		if (p0 == tj)
		{
			int j = 0;
			while (j < p && text[i + j] == pattern[j])
			{
				j++;
			}
			if (j == p)
			{
				found->push_back(i);
			}
		}
		if (i < t - p)
		{
			tj = ((tj - text[i] * h) * d + text[i + p]) % r;
			if (tj < 0)
			{
				tj = tj + r;
			}
		}
	}
	return (*found);
}
