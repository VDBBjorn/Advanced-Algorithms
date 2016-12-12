#pragma once
#include <vector>

using namespace std;

class KnuthMorrisPratt
{
public:
	KnuthMorrisPratt(string prefix, string text): prefix(prefix), text(text)
	{
		prefixVector = vector<int>(prefix.size(), 0);
	}

	vector<int> Execute();
private:
	void CalculatePrefixFunction();
	string prefix;
	string text;
	vector<int> prefixVector;
};

inline vector<int> KnuthMorrisPratt::Execute()
{
	CalculatePrefixFunction();
	vector<int> results;
	int i = 0, j = 0;
	while (i < text.size()) {
		while (text[i] == prefix[j])
		{
			if (j == (prefix.size() - 1)) {
				results.push_back(i - prefix.size() + 1);
				i++;
				j = 0;
			}
			else {
				i++; j++;
			}
		}
		if (j != 0) {
			j = prefixVector[j - 1];
		}
		else
		{
			i++;
		}
	}

	return results;
}

inline void KnuthMorrisPratt::CalculatePrefixFunction()
{
	int j = 0;
	if (prefix.size() <= 0)
		return;
	prefixVector[0] = 0;
	for (int i = 1; i < prefix.size(); i++)
	{
		if (prefix[j] == prefix[i])
		{
			prefixVector[i] = j + 1;
			j++;
		}
		else
		{
			if (j == 0)
			{
				prefixVector[i] = 0;
			}
			else
			{
				while(prefix[i] != prefix[j] && j>0)
				{
					j = prefixVector[j - 1];					
				}
				if (prefix[i] == prefix[j])
				{
					prefixVector[i] = j + 1;
				}
			}
		}
	}
}