#include <iostream>
#include "RedBlackTree.h"
using namespace std;

int main()
{
	RedBlackTree<int, double> tree;

	cout << "depth " << tree.depth() << endl;
	tree.AddBottomUp(5, 22.3);

	tree.AddBottomUp(9, 32.3);
	cin.get();
	return 0;
}