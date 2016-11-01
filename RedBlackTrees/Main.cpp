#include <iostream>
#include "../Tree/Tree.h"
#include "RedBlackTreeTopDown.h"
using namespace std;

int main()
{
	RedBlackTreeTopDown<int, double> tree;
	tree.Add(5, 3.1);
	tree.Add(2, 3.7);
	tree.Add(7, 3.7);
	tree.Add(4, 3.1);
	/*tree.Add(10, 3.9);*/
	tree.Add(1, 3.7);
	/*tree.Add(6, 3.7);*/

	cout << "Depth: " << tree.Depth() << endl;
	cout << "Number of nodes: " << tree.NumberOfNodes() << endl;
	cout << tree << endl;

	cin.get();
	return 0;
}
