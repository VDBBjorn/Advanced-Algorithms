#pragma once
#include "Graaf.h"
#include <queue>

using namespace std;

template <RichtType RT>
class ConnectedComponentSearcher
{
public:
	explicit ConnectedComponentSearcher(Graaf<RT>&);
	virtual bool IsConnected() = 0;

	virtual ~ConnectedComponentSearcher()
	{
	}

	Graaf<RT> Graph;
	vector<bool> DepthFirstSearch();
};

class DirectedCompontentSearcher : public ConnectedComponentSearcher<GERICHT>
{
public:
	~DirectedCompontentSearcher()
	{
	}
	explicit DirectedCompontentSearcher(Graaf<GERICHT>& graaf);
	bool IsConnected() override;
	bool IsStronglyConnected();
};

class UndirectedComponentSearcher : public ConnectedComponentSearcher<ONGERICHT>
{
public:
	~UndirectedComponentSearcher()
	{
	}
	explicit UndirectedComponentSearcher(Graaf<ONGERICHT>& graaf);
	bool IsConnected() override;
};

template <RichtType RT>
ConnectedComponentSearcher<RT>::ConnectedComponentSearcher(Graaf<RT>& g)
{
	Graph = g;
}

inline DirectedCompontentSearcher::DirectedCompontentSearcher(Graaf<GERICHT>& graaf): ConnectedComponentSearcher<GERICHT>(graaf)
{
}

inline bool DirectedCompontentSearcher::IsConnected()
{
	vector<bool> discovered = DepthFirstSearch();
	for (int i = 0; i < discovered.size(); i++)
	{
		if (discovered[i] == false)
			return false;
	}
	return true;
}

inline bool DirectedCompontentSearcher::IsStronglyConnected()
{
	return false;
}

inline UndirectedComponentSearcher::UndirectedComponentSearcher(Graaf<ONGERICHT>& graaf): ConnectedComponentSearcher<ONGERICHT>(graaf)
{
}

inline bool UndirectedComponentSearcher::IsConnected()
{
	vector<bool> discovered = DepthFirstSearch();
	for (int i = 0; i < discovered.size(); i++)
	{
		if (discovered[i] == false)
			return false;
	}
	return true;
}

template <RichtType RT>
vector<bool> ConnectedComponentSearcher<RT>::DepthFirstSearch()
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
