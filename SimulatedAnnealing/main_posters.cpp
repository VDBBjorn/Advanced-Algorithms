#include "Sodor.h"
#include <vector>
#include <iostream>
using namespace std;

int main()
{
	vector<City> cities{
		City(0, 1, 5),
		City(1, 5, 9),
		City(2, 6, 0),
		City(3, 1, 4),
		City(4, 9, 3),
		City(5, 1, 4),
		City(6, 3, 6),
		City(7, 7, 8),
		City(8, 1, 9),
		City(9, 10, 7)
	};
	PostersInSodor sodor(cities);
	vector<int> solution = sodor.oplossing();
	cout << "Solution: ";
	for(auto city: solution)
	{
		cout << city << " ";
	}
	cout << endl;
	cin.get();
}
