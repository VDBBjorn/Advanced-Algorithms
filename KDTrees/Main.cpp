#include "KDTree.h"
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

void split(const std::string& s, char delim, std::vector<std::string>& elems)
{
	std::stringstream ss;
	ss.str(s);
	std::string item;
	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
}

std::vector<std::string> split(const std::string& s, char delim)
{
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

int main()
{
	KDTree tree;
	/*string line;
	ifstream myfile("input.txt");
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			vector<string> elements = split(line, ' ');
			Point p(stoi(elements[0]), stoi(elements[1]));
			tree.Add(p);
		}
		myfile.close();
	}
	else cout << "Unable to open file";

	cout << "done reading" << endl;*/

	tree.Add(Point(3, 7));
	tree.Add(Point(8, 1));
	tree.Add(Point(6, 6));
	tree.Add(Point(2, 6));
	tree.Add(Point(1, 7));
	tree.Add(Point(8, 6));
	tree.Add(Point(5, 9));

	cout << tree << endl;
	cin.get();
	return 0;
}
