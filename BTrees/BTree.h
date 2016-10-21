#pragma once
#include <vector>
#include "Disk.h"
#include <stack>
//betekenis m: orde van de B-tree (maximaal m kinderen en m-1 sleutels)

using namespace std;

template <class T, class D, unsigned int m>
class BTree;
template <class T, class D, unsigned int m>
class BNode;

template <class T, class D, unsigned int m>
class BTree
{
	typedef BNode<T, D, m> Node;
public:
	explicit BTree(Disk<Node>& disk)
		: disk(disk)
	{
		root.k = 0;
		root.isLeaf = false;
		rootIndex = disk.Write(root);
	}

	void Put(T key, D val);
	void Search(T key);
private:
	Disk<BNode<T,D,m>>& disk;
	Node<T,D,m> root;
	blockindex rootIndex;
	D Search(T key, stack<blockindex>& path);
};

template <class T, class D, unsigned m>
void BTree<T, D, m>::Put(T key, D val)
{
	//TODO
}

template <class T, class D, unsigned m>
void BTree<T, D, m>::Search(T key)
{
	//TODO
}

template <class T, class D, unsigned m>
D BTree<T, D, m>::Search(T key, stack<blockindex>& path)
{
	return {};
}

template <class T, class D, unsigned int m>
class BNode
{
private:
	friend class BTree<T, D, m>;
public:
	vector<T> keys;
	vector<D> data;	

	/// <summary>
	/// The current number of keys
	/// </summary>
	unsigned int k;

	bool isLeaf;
};
