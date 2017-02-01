#include "Graaf.h"
#include "Flow.h"
#include <iostream>


int main()
{
	GraafMetTakdata <GERICHT, double> gr(9);

	gr.voegVerbindingToe(0, 1, 5);
	gr.voegVerbindingToe(0, 2, 9);
	gr.voegVerbindingToe(1, 3, 1);
	gr.voegVerbindingToe(2, 5, 3);
	gr.voegVerbindingToe(1, 4, 5);
	gr.voegVerbindingToe(3, 6, 10);
	gr.voegVerbindingToe(4, 6, 1);
	gr.voegVerbindingToe(4, 5, 20);
	gr.voegVerbindingToe(5, 7, 4);
	gr.voegVerbindingToe(7, 8, 6);
	gr.voegVerbindingToe(6, 8, 6);
	gr.voegVerbindingToe(7, 6, 1);
	Pad<int> pad;
	int van = 0;
	int min = 999;
	int minvan = 0;
	int minnaar = 0;
	for (int i = 1; i<gr.aantalKnopen(); i++) {
		int naar = i;
		Stroomnetwerk<double> sn(gr, van, naar);
		int getal = sn.geefCapaciteit();
		cout << van << " -(" << getal << ")-> " << naar << endl;
		if (getal<min) {
			min = getal;
			minvan = van;
			minnaar = naar;
		}
		van = naar;
	}
	//	Stroomnetwerk<double> sn(gr,39,40);
	//    cout<<sn<<endl;
	cout << "minimum:" << endl;
	cout << minvan << " -(" << min << ")-> " << minnaar << endl;
	//    assert(sn.geefCapaciteit()==13);
	cin.get();
	return 0;
}
