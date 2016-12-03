#include "ConnectedComponentSearcher.h"
#include <fstream>
#include "EulerCircuitSearcher.h"

void ConnectedComponentsMain()
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
}

void EulerCircuitMain()
{
	Graaf<ONGERICHT> graaf3;
	ifstream is2("graaf2.txt");
	graaf3.lees(is2);
	EulerCircuitSearcher<ONGERICHT> searcher(graaf3);
	cout << (searcher.IsEulerGraph() ? "IS EULER" : "IS NO EULER") << endl;
	cin.get();
}


int main()
{
	//ConnectedComponentsMain();
	EulerCircuitMain();
}
