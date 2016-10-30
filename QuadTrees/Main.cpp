#include <iostream>
using namespace std;

#include "quadtree.h"

int main() {
	QuadTree quadtree;

	for (int i = 0; i<30; i++) {
		Point p;
		p.x = i;
		p.y = i*2;
		quadtree.Add(p);
	}
	Point p;
	p.x = 20;
	p.y = 40;
	auto result = quadtree.Search(p);
	cout << *result << endl;
	cout << "Diepte: " << quadtree.Depth() << endl;
	cout << quadtree << endl;
	cin.get();
	return 0;
}