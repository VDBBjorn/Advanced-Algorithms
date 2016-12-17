#include "BoyerMooreBadCharHeuristic.h"
#include <iostream>
#include "BoyerMooreHorspool.h"

string print(string text, int occurence, int length)
{
	string match = "";
	int j = length;
	while(j > 0)
	{
		match += text[occurence + (length - j)];
		j--;
	}
	return match;
}

int main()
{
	string text = "dit is een test en dit is een ander testje";
	string pattern = "test";
	BoyerMooreHorspool bm(text,pattern);
	vector<int>* occurences = bm.Search();
	for (auto occurence : *occurences)
	{
		cout << occurence << "\t" << print(text, occurence, pattern.length()) << endl;
	}
	cin.get();
	return 0;
}
