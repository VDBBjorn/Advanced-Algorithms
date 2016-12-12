#pragma once
#include <string>
#include <vector>

using namespace std;

class KnuthMorrisPratt
{
public:
	KnuthMorrisPratt(string prefix, string text):prefix(prefix), text(text)
	{
		
	}

	vector<int>& Execute() const;
private:
	string prefix;
	string text;
};

inline void PrefixFunction(vector<int>& v, string prefix)
{
	int j=0;
	if (prefix.size() <= 0)
		return;
	v[0] = 0;
	for(int i=1; i<prefix.size();i++)
	{
		if(prefix[j] == prefix[i])
		{
			v[i] = v[j] + 1;
			j++;
		}
		else
		{
			if (j == 0) {
				v[i] = 0;
			}
			else
			{
				j = v[j - 1];
				if(prefix[i] == prefix[j])
				{
					v[i] = v[j] + 1;
				}
				else
				{
					v[i] = 0;
				}
			}
		}
	}
}

inline vector<int>& KnuthMorrisPratt::Execute() const
{
	vector<int> prefixVector(prefix.size(),-1);
	PrefixFunction(prefixVector, prefix);

	return prefixVector;
}
