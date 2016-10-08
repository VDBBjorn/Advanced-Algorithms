#include <iostream>
#include "headers/RedBlackTree.h"
using namespace std;

int main()
{
    RedBlackTree<int,double> tree;
	
    cout << "depth " << tree.depth() << endl;	
    return 0;
}