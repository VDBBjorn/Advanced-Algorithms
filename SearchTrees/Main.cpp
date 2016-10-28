#include <iostream>
#include "SearchTree.h"
using namespace std;


int main()
{
	SearchTree<int, double> tree;
	tree.Add(44, 3.1);
	tree.Add(30, 3.1);
	tree.Add(16, 3.9);
	tree.Add(39, 3.7);
	tree.Add(76, 3.3);


	cout << "Depth " << tree.Depth() << endl;
	cout << tree << endl;

	tree.Rotate(false);

	cout << "Depth " << tree.Depth() << endl;
	cout << tree << endl;

	tree.Rotate(true);

	cout << "Depth " << tree.Depth() << endl;
	cout << tree << endl;
	cin.get();
	return 0;
}
