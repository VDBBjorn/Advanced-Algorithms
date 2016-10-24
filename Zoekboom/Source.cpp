#include <iostream>
#include "zoekboom.h"
using namespace std;


int main()
{
	Zoekboom<int, double> tree;
	tree.voegtoe(5, 3.1);
	tree.voegtoe(4, 3.1);
	tree.voegtoe(10, 3.9);
	tree.voegtoe(2, 3.7);
	tree.voegtoe(7, 3.3);


	cout << tree << endl;

	cin.get();
	return 0;
}
