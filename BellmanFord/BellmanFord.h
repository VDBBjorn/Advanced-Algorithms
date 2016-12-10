#ifndef __BELLMAN_FORD
#define __BELLMAN_FORD

#include "Graaf.h"
#include <queue>
#include <limits>
#include <iostream>
using namespace std;

typedef map<int, int>  Knoop;
int INF = std::numeric_limits<int>::max();

inline ostream & operator<<(ostream& os, vector<int> & v) {
	for (auto branch : v) {
		os << branch << " ";
	}
	os << endl;
	return os;
}

class BellmanFord {
public:
	BellmanFord(GraafMetTakdata<GERICHT, int> &);
	vector<int> GetCosts() const;
protected:
	vector<int> costs;
};

inline BellmanFord::BellmanFord(GraafMetTakdata<GERICHT, int> & graaf) {
	int iteraties = graaf.aantalKnopen() - 1;
	costs = *(new vector<int>(graaf.aantalKnopen()));
	costs[0] = 0;
	queue<int> q;
	queue<int> q2;
	q.push(0);
	for (int i = 1; i<graaf.aantalKnopen(); i++) {
		costs[i] = INF;
		q.push(i);
	}
	int iteratie = 0;
	while (iteratie < iteraties && !q.empty()) {
		while (!q.empty()) {
			int j = q.front();
			q.pop();
			if (costs[j] == INF) {
				continue;
			}
			Knoop k = graaf[j];
			for (auto it = k.begin(); it != k.end(); it++) {
				int neighbor = it->first;
				int weight = *(graaf.geefTakdata(j, neighbor));
				int total = weight + costs[j];
				if (costs[neighbor] > total) {
					costs[neighbor] = total;
					if (iteratie == iteraties - 1) {
						cout << "a negative cycle was found, stopping execution" << endl;
						return;
					}
					q2.push(neighbor);
				}
			}
		}
		cout << costs;
		q = q2;
		q2 = queue<int>();
		iteratie++;
	}
}

inline vector<int> BellmanFord::GetCosts() const
{
	return costs;
}

#endif