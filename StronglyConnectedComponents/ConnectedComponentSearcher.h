#pragma once
#include "Graaf.h"
#include <queue>

using namespace std;
typedef map<int, int> Knoop;

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
	static bool IsStronglyConnected();
	vector<vector<int>>& FindConnectedComponents();
protected:
	Graaf<GERICHT>& Inverse();
	vector<vector<int>>& FindComponents(stack<int>&);
	void DepthFirstSearchComponents(int i, vector<bool>& visited, vector<int>* connected);
};

class UndirectedComponentSearcher : public ConnectedComponentSearcher<ONGERICHT>
{
public:
	~UndirectedComponentSearcher()
	{
	}

	explicit UndirectedComponentSearcher(Graaf<ONGERICHT>& graaf);
	bool IsConnected() override;
	void FindBridges();
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

inline void PostOrderDFS(const Graaf<GERICHT>& g, int i, stack<int>* q, vector<bool>& visited)
{
	if (!visited[i])
	{
		visited[i] = true;
		q->push(i);
		map<int, int> knoop = g[i];
		for (auto it = knoop.begin(); it != knoop.end(); it++)
		{
			int neighbor = it->first;
			PostOrderDFS(g, neighbor, q, visited);
		}
	}
}

inline stack<int>& PostOrderDFS(const Graaf<GERICHT>& g)
{
	stack<int>* postorder = new stack<int>();
	vector<bool> visited(g.aantalKnopen());
	int i = 0;
	while (postorder->size() < g.aantalKnopen())
	{
		PostOrderDFS(g, i++, postorder, visited);
	}
	return *postorder;
}

/// <summary>
/// Determines whether a directed graph is strongly connected.
/// </summary>
/// <returns>
///   <c>true</c> if a directed graph is strongly connected; otherwise, <c>false</c>.
/// </returns>
inline bool DirectedCompontentSearcher::IsStronglyConnected()
{
	return false;
}

/// <summary>
/// Finds the Connected Components of a Graph
/// Algorithm of Kosaraju
/// 1) Construct the inverse graph
/// 2) DFS the inverse graph with post-order numbering
/// 3) DFS the original graph, getting the highest post-order numbers from the stack as starting points
/// </summary>
/// <returns>
///   <c>true</c> if the graph is strongly connected; otherwise, <c>false</c>.
/// </returns>
inline vector<vector<int>>& DirectedCompontentSearcher::FindConnectedComponents()
{
	Graaf<GERICHT> inverse = Inverse();
	stack<int> postorder = PostOrderDFS(inverse);
	return FindComponents(postorder);
}

inline Graaf<GERICHT>& DirectedCompontentSearcher::Inverse()
{
	Graaf<GERICHT>* invg = new Graaf<GERICHT>();
	for (int i = 0; i < Graph.aantalKnopen(); i++)
	{
		invg->voegKnoopToe();
	}
	for (int i = 0; i < Graph.aantalKnopen(); i++)
	{
		map<int, int> knoop = Graph[i];
		for (auto it = knoop.begin(); it != knoop.end(); it++)
		{
			invg->voegVerbindingToe(it->first, i);
		}
	}
	return (*invg);
}

inline vector<vector<int>>& DirectedCompontentSearcher::FindComponents(stack<int>& postorder)
{
	vector<vector<int>>* tree = new vector<vector<int>>();
	vector<bool> visited(Graph.aantalKnopen(),false);
	while (!postorder.empty())
	{
		vector<int>* connected = new vector<int>();
		int i = postorder.top();
		postorder.pop();
		cout << i << endl;
		DepthFirstSearchComponents(i, visited, connected);
		tree->push_back(*connected);
	}

	return *tree;
}

inline void DirectedCompontentSearcher::DepthFirstSearchComponents(int i, vector<bool>& visited, vector<int>* connected)
{
	if (!visited[i])
	{
		visited[i] = true;
		connected->push_back(i);
		map<int, int> knoop = Graph[i];
		for (auto it = knoop.begin(); it != knoop.end(); it++)
		{
			int neighbor = it->first;
			DepthFirstSearchComponents(neighbor, visited, connected);
		}
	}
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
inline void RecFindBridges(vector<bool>& discovered, vector<int>& parent, vector<int>& pre, vector<int>& lowest, int num, int i, Graaf<RT>& g)
{
	discovered[i] = true;
	pre[i] = num++;
	lowest[i] = pre[i]; // initialisatie: elke knoop bereikt minstens zichzelf

	// Alle buren ophalen en over lopen
	Knoop buren = g[i];
	map<int, int>::iterator buren_it = buren.begin();
	while (buren_it != buren.end())
	{
		auto buur = buren_it->first;
		if (!discovered[buur])
		{
			// boomtak, dus kind van i
			parent[buur] = i;
			RecFindBridges(discovered, parent, pre, lowest, num, buur, g); // recursief oproepen voor buur
			if (lowest[buur] < lowest[i])
				lowest[i] = lowest[buur]; // nieuw minimum
			else if (lowest[buur] >= pre[i]) // voor bruggen: laagst[buur] > pre[i]
				cout << "Knoop " << i << " is brug voor knoop " << buur << endl;
		}
		else
		{
			if (buur != parent[i]) // eventueel terugverbinding
				if (pre[buur] < lowest[i]) // dan is zeker pre[buur] < pre[i] : dus terugverbinding
					lowest[i] = pre[buur]; // nieuw minimum
		}
		++buren_it;
	}
}

/// <summary>
/// Finds the bridges of a component.
/// </summary>
/// <returns></returns>
inline void UndirectedComponentSearcher::FindBridges()
{
	vector<bool> discovered(Graph.aantalKnopen());
	vector<int> parents(Graph.aantalKnopen());
	vector<int> pre(Graph.aantalKnopen());
	vector<int> lowest(Graph.aantalKnopen());
	for (int i = 0; i < Graph.aantalKnopen(); i++)
	{ // initialisatie
		discovered[i] = false;
		parents[i] = -1; // nog geen ouders
	}
	auto num = 0; // initialisatie preordernummering
	RecFindBridges(discovered, parents, pre, lowest, num, 0, Graph); 
}

template <RichtType RT>
vector<bool> ConnectedComponentSearcher<RT>::DepthFirstSearch()
{
	vector<bool> discovered(Graph.aantalKnopen(),false);
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
