#include <iostream>
#include "SplayTree.h"
using namespace std;

void AddTest()
{

	SplayTree<int, double> tree;

	cout << tree << endl;
	tree.Add(5, 3.1);

	cout << tree << endl;
	tree.Add(4, 3.1);

	cout << tree << endl;
	tree.Add(10, 3.9);

	cout << tree << endl;
	tree.Add(2, 3.7);

	cout << tree << endl;
	tree.Add(7, 3.3);

	cout << tree << endl;
	tree.Add(6, 3.9);

	cout << tree << endl;
	tree.Add(11, 3.7);

	cout << tree << endl;
	tree.Add(1, 3.3);

	cout << tree << endl;

	cout << "depth " << tree.depth() << endl;
	cout << tree << endl;
}

int main()
{
	AddTest();
	cin.get();
	return 0;
}
