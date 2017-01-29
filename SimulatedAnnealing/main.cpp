#include <iostream>
#include <ctime>
#include "SmallestValue.h"

int main() {
	srand(time(NULL));
	std::cout << "" << std::endl; // prints 
	SmallestValue<double> kw(0, 0);
	double * oplossing = kw.newOplossing();
	std::cout << "f(" << kw.x << "," << kw.y << ") = " << *oplossing << " (in " << kw.iteraties << " iteraties)" << std::endl;
	std::cin.get();
	return 0;
}
