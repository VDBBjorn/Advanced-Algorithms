#include "KnuthMorrisPratt.h"
#include <iostream>

ostream& operator<<(ostream& os, const vector<int>& v) {
	for(int i=0; i<v.size();i++)
	{
		os << v[i] << " ";
	}
	return os;
}

int main()
{
	KnuthMorrisPratt kmp("abcdabca", "abcdabcx");
	vector<int> result = kmp.Execute();

	cout << result << endl;
	cin.get();
	return 0;
}
