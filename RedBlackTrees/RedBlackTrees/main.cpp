#include <iostream>
#include "RedBlackTree.h"
using namespace std;

int main()
{
	RedBlackTree<int, double> tree;

	cout << "depth " << tree.depth() << endl;
	cin.get();
	return 0;
}