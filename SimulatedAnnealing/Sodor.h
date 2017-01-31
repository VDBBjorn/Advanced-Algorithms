#pragma once
#include "SimulatedAnnealing.h"
#include <vector>
#include <ctime>
#include <iostream>

using namespace std;

class City
{
public:
	City(int id, int x, int y) : id(id), x(x), y(y)
	{
	}

	double Distance(City city) const
	{
		double horizontal = x - city.x;
		double vertical = y - city.y;
		return sqrt(pow(horizontal, 2) + pow(vertical, 2));
	}

	int GetId() const
	{
		return id;
	}

	int GetX() const
	{
		return x;
	}

	int GetY() const
	{
		return y;
	}

protected:
	int id;
	int x, y;
};

class CityException : public std::logic_error
{
public:
	CityException(const std::string& message) : std::logic_error(message)
	{
	}
};

inline std::ostream& operator<<(std::ostream& os, const CityException& exc)
{
	return os << exc.what();
}


class PostersInSodor: public SimulatedAnnealing<vector<int>>
{
public:
	explicit PostersInSodor(vector<City>& cities): cities(cities)
	{
		srand(time(nullptr));
		distance = -1;
	}

protected:

	double T0() override;
	bool terminateCond() override;
	double f(const vector<int>&) override;
	vector<int> initialSolution() override;
	vector<int> pickAtRandom(const vector<int>&) override;
	double p(double T, double deltaf) override;
	void updateT() override;

private:
	double distance;
	City GetCity(int id);
	vector<City> cities;
	int iterationsNotChanged = 0;
	const int ITERATIONS = 100;
};

inline bool PostersInSodor::terminateCond()
{
	if(T < 0.01)
	{
		return true;
	}
	if(distance == -1)
	{
		distance = f(s);
		iterationsNotChanged = 0;
	}
	else
	{
		double newDistance = f(s);
		if(newDistance == distance)
		{
			iterationsNotChanged++;
			if(iterationsNotChanged == ITERATIONS)
			{
				return true;
			}
		}
		else
		{
			distance = newDistance;
			iterationsNotChanged = 0;
		}
	}
	return false;
}

inline double PostersInSodor::f(const vector<int>& tempSolution)
{
	double total = 0;
	for(int i=1; i<tempSolution.size(); i++)
	{
		City c1 = GetCity(tempSolution[i - 1]);
		City c2 = GetCity(tempSolution[i]);
		total += c1.Distance(c2);
	}
	return total;
}

inline vector<int> PostersInSodor::initialSolution()
{
	vector<int> solution;
	for (City city: cities)
	{
		solution.push_back(city.GetId());
	}
	return solution;
}

inline vector<int> PostersInSodor::pickAtRandom(const vector<int>& tempSolution)
{
	vector<int> * newSolution = new vector<int>();
	for (auto temp: tempSolution)
	{
		newSolution->push_back(temp);
	}
	//swap two random cities
	int first = tempSolution[rand() % tempSolution.size()];
	int second = tempSolution[rand() % tempSolution.size()];
	while (first == second)
	{
		second = tempSolution[rand() % tempSolution.size()];
	}
	(*newSolution)[first]  = tempSolution[second];
	(*newSolution)[second] = tempSolution[first];
	return (*newSolution);
}

inline double PostersInSodor::p(double T, double deltaf)
{
	return exp(-deltaf / T);
}

inline double PostersInSodor::T0()
{
	return 10000.0;
}

inline void PostersInSodor::updateT()
{
	T *= 0.99;
}

inline City PostersInSodor::GetCity(int id)
{
	for (City city:cities)
	{
		if (city.GetId() == id)
		{
			return city;
		}
	}
	
}
