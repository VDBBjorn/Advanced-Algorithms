#include "ConnectedComponentSearcher.h"
#include <fstream>

int main()
{
	Graaf<GERICHT> graaf;
	ifstream is("graaf1.txt");
	graaf.lees(is);
	DirectedCompontentSearcher searcher(graaf);
	cout << "This graph is " << (searcher.IsConnected() ? "connected!" : "not connected!") << endl;
	cin.get();
	return 0;
}
