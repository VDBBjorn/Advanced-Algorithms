#pragma once
#include "Graaf.h"
#include <queue>

using namespace std;
typedef map<int, int> Knoop;

template <RichtType RT>
class EulerCircuitSearcher
{
public:
	explicit EulerCircuitSearcher(Graaf<RT>&);
	virtual ~EulerCircuitSearcher();

	vector<int> GetEulerPath();

	bool IsEulerGraph();
	bool IsConnected();
	bool IsEveryNodeEven();
protected: 
	Graaf<RT> Graph;
	vector<bool> DepthFirstSearch();
};

template <RichtType RT>
EulerCircuitSearcher<RT>::EulerCircuitSearcher(Graaf<RT>& g)
{
	Graph = g;
}

template <RichtType RT>
EulerCircuitSearcher<RT>::~EulerCircuitSearcher()
{
}

/* 
*/
template <RichtType RT>
vector<int> EulerCircuitSearcher<RT>::GetEulerPath()
{
	
}

template <RichtType RT>
bool EulerCircuitSearcher<RT>::IsEveryNodeEven()
{
	vector<bool> discovered(Graph.aantalKnopen());
	for (int i = 0; i < discovered.size(); i++)
	{
		discovered[i] = false;
	}
	queue<int> q;
	q.push(0);
	while (!q.empty())
	{
		int knoopnr = q.front();
		q.pop();
		map<int, int> k = Graph[knoopnr];
		if (k.size() % 2 != 0) return false;
		for (map<int, int>::const_iterator it = Graph[knoopnr].begin(); it != Graph[knoopnr].end(); ++it)
		{
			int buurnr = it->first();
			if (!discovered[buurnr])
			{
				discovered[buurnr] = true;
				q.push(buurnr);
			}
		}
	}
	return true;
}

template <RichtType RT>
vector<bool> EulerCircuitSearcher<RT>::DepthFirstSearch()
{
	vector<bool> discovered(Graph.aantalKnopen());
	for (int i = 0; i < discovered.size(); i++)
	{
		discovered[i] = false;
	}
	queue<int> q;
	q.push(0);
	discovered[0] = true;
	while (!q.empty())
	{
		int knoopnr = q.front();
		q.pop();
		for (map<int, int>::const_iterator it = Graph[knoopnr].begin(); it != Graph[knoopnr].end(); ++it)
		{
			int buurnr = it->first;
			if (!discovered[buurnr])
			{
				discovered[buurnr] = true;
				q.push(buurnr);
			}
		}
	}
	return discovered;
}
/// 
/// (1) Een samenhangende (multi)graaf G is een Eulergraaf.
/// (2) De graad van elke knoop van G is even.
/// (3) De verbindingen van G kunnen onderverdeeld worden in lussen. (Deze verdeling
/// 	is een partitie : elke verbinding behoort tot een enkele lus.)
template <RichtType RT>
bool EulerCircuitSearcher<RT>::IsEulerGraph()
{
	// (1) Een samenhangende (multi)graaf G is een Eulergraaf.
	if (IsConnected()) return true;
	return false;
}

template <RichtType RT>
bool EulerCircuitSearcher<RT>::IsConnected()
{
	vector<bool> discovered = DepthFirstSearch();
	for (int i = 0; i < discovered.size(); i++)
	{
		if (discovered[i] == false)
			return false;
	}
	return true;
}
