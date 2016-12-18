#pragma once
#include <string>
#include <vector>

using namespace std;

class KarpRabinal
{
public:
	KarpRabinal(string text, string pattern, int radix);
	KarpRabinal(string text, string pattern);
	vector<int>* Search();
private:
	string text;
	string pattern;
	int radix;
};

inline KarpRabinal::KarpRabinal(string text, string pattern, int radix): text(text), pattern(pattern), radix(radix)
{
}

inline KarpRabinal::KarpRabinal(string text, string pattern): KarpRabinal(text, pattern, 101)
{
}

inline vector<int>* KarpRabinal::Search()
{
}
