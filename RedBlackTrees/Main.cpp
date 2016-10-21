#include <iostream>
#include "../Tree/Tree.h"
#include "RedBlackTree.h"
using namespace std;

int main()
{
	RedBlackTree<int, double> tree;
	tree.Add(5, 3.1);
	tree.Add(4, 3.1);
	tree.Add(10, 3.9);
	tree.Add(2, 3.7);
	tree.Add(7, 3.3);

	cout << "Depth " << tree.Depth() << endl;
	cout << "number " << tree.NumberOfNodes() << endl;
	cout << tree << endl;
	cin.get();
	return 0;
}
