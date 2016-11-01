#include <iostream>
#include "BTree.h"
using namespace std;
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// gebruikte sleutel, kan een klasse zijn maar struct werkt ook.
struct Word {
	string w;
	// vergelijkingsoperator (kleiner dan)
	bool operator<(const Word &t) const {
		int o = w.compare(t.w);
		return (o < 0) ? true : false;
	}

	bool operator==(const Word &t) const {
		int o = w.compare(t.w);
		return (o == 0);
	}
};

int main() {

	Disk<BNode<Word, int, 666> > disk;
	BTree<Word, int, 666> tree(disk);

	ifstream ifile;
	ifile.open("clinton.txt");
	if (!ifile) {
		cout << "Unable to open file";
		exit(1); 
	}

	string woord;
	ifile >> woord;
	while (!ifile.fail()) {
		Word my_woord;
		my_woord.w = woord;
		DataLocation location = tree.Search(my_woord);
		BNode<Word, int, 666> node;
		disk.Read(node, location.blockindex_);
		tree.Put(my_woord, node.data[location.keyindex]);
		ifile >> woord;
	}
	ifile.close();
	cout << "Done";

	tree.GetTopWords(20);

	cin.get();
	return 0;
}