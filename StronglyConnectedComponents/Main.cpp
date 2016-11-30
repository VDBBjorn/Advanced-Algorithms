#include "ConnectedComponentSearcher.h"
#include <fstream>

int main()
{
	Graaf<GERICHT> graaf;
	ifstream is("graaf1.txt");
	graaf.lees(is);
	DirectedCompontentSearcher searcher(graaf);
	cout << "This graph is " << (searcher.IsConnected() ? "connected!" : "not connected!") << endl;

	Graaf<ONGERICHT> graaf2;
	ifstream is2("graaf2.txt");
	graaf2.lees(is2);
	UndirectedComponentSearcher usearcher(graaf2);
	usearcher.FindBridges();
	cin.get();
	return 0;
}
