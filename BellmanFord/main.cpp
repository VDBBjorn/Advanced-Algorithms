#include "BellmanFord.h"

int main() {
	cout << "Bellman-ford algorithm" << endl;
	GraafMetTakdata<GERICHT, int> graaf(6);
	graaf.voegVerbindingToe(0, 1, 10);
	graaf.voegVerbindingToe(0, 5, 8);
	graaf.voegVerbindingToe(5, 4, 1);
	graaf.voegVerbindingToe(4, 1, -4);
	graaf.voegVerbindingToe(4, 3, -1);
	graaf.voegVerbindingToe(3, 2, -2);
	graaf.voegVerbindingToe(2, 1, 1);
	graaf.voegVerbindingToe(1, 3, 2);
	//graaf.schrijf(cout);
	BellmanFord * bf = new BellmanFord(graaf);
	cin.get();
}
