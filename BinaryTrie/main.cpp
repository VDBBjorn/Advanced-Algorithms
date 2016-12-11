#include "BinaryTrie.h"

int main()
{
	BTrie trie;
	BitPattern b = BitPattern::eenbit(1);
	cout << b << endl;
	trie.Add(b);
	BitPattern c = BitPattern::eenbit(5);
	cout << c << endl;
	trie.Add(b);
	cin.get();
	return 0;
}
