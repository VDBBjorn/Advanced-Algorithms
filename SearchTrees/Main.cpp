#include <iostream>
#include "../Tree/Tree.h"
#include "SearchTree.h"
using namespace std;


int main()
{
	SearchTree<int, double> tree;
	tree.Add(5, 3.1);
	tree.Add(4, 3.1);
	tree.Add(10, 3.9);
	tree.Add(2, 3.7);
	tree.Add(7, 3.3);


	cout << "depth " << tree.depth() << endl;
	cout << tree << endl;

	tree.Rotate(true);

	cout << "depth " << tree.depth() << endl;
	cout << tree << endl;
	cin.get();
	return 0;
}
