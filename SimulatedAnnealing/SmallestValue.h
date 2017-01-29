#pragma once
#include "SimulatedAnnealing.h"
#include <iostream>

template<class S>
class SmallestValue : public SimulatedAnnealing<S> {
public:
	SmallestValue(S x, S y);
	S x, y;

protected:
	~SmallestValue() = default;
	S calculateFunction(S x, S y);

	double T0() override; 
	bool terminateCond() override; 
	double f(const S&) override; 
	S initialSolution() override;
	S* newpickAtRandom(const S*);
	double p(double T, double deltaf) override; 
	void updateT() override;
};

template<class S>
SmallestValue<S>::SmallestValue(S x, S y) :
	x(x), y(y) {
	SimulatedAnnealing<S>::iteraties = 1;
}


template<class S>
double SmallestValue<S>::f(const S&s) {
	return s;
}

template<class S>
S SmallestValue<S>::initialSolution() {
	x = 0;
	y = 0;
	S solution = calculateFunction(x, y);
	return solution;
}

template<class S>
double SmallestValue<S>::T0() {
	return 1000.0;
}

template<class S>
double SmallestValue<S>::p(double T, double deltaf) {
	double exponent = (deltaf / T);
	return exp(-exponent);
}

template<class S>
void SmallestValue<S>::updateT() {
	double temp = SimulatedAnnealing<S>::T - 1;
	std::cout<<"andere temperatuur: "<<temp<<std::endl;
	if (temp > 0) {
		SimulatedAnnealing<S>::T = temp;
	}
	else {
		SimulatedAnnealing<S>::T = 0.0;
	}
}

template<class S>
bool SmallestValue<S>::terminateCond() {
	S endresult(0.11);
	return ((*(SimulatedAnnealing<S>::s) <= endresult) || SimulatedAnnealing<S>::T <= 0);
}

template<class S>
S SmallestValue<S>::calculateFunction(S x, S y) {
	S _function = 1000 * (1 + cos((x / 5 + y / 7.0)))
		+ (1 + cos(0.01 * (x + y))) + 0.0001 * (sqrt(x * x + y * y));
	S* solution = new S(_function);
	std::cout << "calculated:" << solution << std::endl;
	return &solution;
}

template<class S>
S* SmallestValue<S>::newpickAtRandom(const S*) {
	S * goX = calculateFunction(x + 1, y);
	S * goY = calculateFunction(x, y + 1);
	if (*goX < *goY) {
		++x;
		return goX;
	}
	else {
		++y;
		return goY;
	}

}