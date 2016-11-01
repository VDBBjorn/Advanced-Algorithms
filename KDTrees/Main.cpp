#include "KDTree.h"
#include <vector>

int main()
{
	KDTree tree;
	tree.Read("input.txt");
	for(auto i=0; i<200; i+=10)
	{
		for(auto j=0; j<200; j+=10)
		{
			Point p = Point(i, j);
			int visited = 0;
			auto closest = tree.GetClosest(p, visited);
			cout << p << " heeft buur " << closest.second->get()->point << " gevonden in " << visited << " doorzochte knopen" << endl;
		}
	}
	cin.get();
	return 0;
}
