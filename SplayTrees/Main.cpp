#include <iostream>
#include "SplayTree.h"
#include "PointGenerator.h"
#include "CsvData.h"
using namespace std;

int aantalDatapunten = 6000;

void AddTest()
{
	CsvData grafiek("data", ',');
	PointGenerator dePuntgenerator;
	SplayTree<double, unsigned int> tree;
	vector<double> dieptedata;
	int aantal = 0;
	for (int i = 0; i<aantalDatapunten; i++) {
		double nupunt = dePuntgenerator.GetPoint();
		tree.Add(nupunt, i);
		aantal++;
		if (aantal * 50 >= aantalDatapunten) {
			dieptedata.push_back(tree.Depth());
			aantal = 0;
		}
	}
	grafiek.voegDataToe(dieptedata);
}

int main()
{
	AddTest();
	cin.get();
	return 0;
}
