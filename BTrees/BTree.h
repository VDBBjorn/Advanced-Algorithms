#pragma once
#include "Disk.h"
#include <stack>
#include <vector>
//betekenis m: orde van de B-tree (maximaal m children en m-1 sleutels)

using namespace std;

template <class T, class D, unsigned int m>
class BTree;
template <class T, class D, unsigned int m>
class BNode;

struct DataLocation
{
	blockindex blockindex_ = 0;
	int keyindex = 0;
	bool found = false;
	stack<blockindex> path;
};

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
	DataLocation Search(T key);
	vector<pair<string,int>> GetTopWords(int number)
	{
		vector<pair<string,int>> result(20);
		//this->
		return result;
	}

private:
	Disk<BNode<T, D, m>>& disk;
	Node root;
	blockindex rootIndex;
};

template <class T, class D, unsigned m>
void BTree<T, D, m>::Put(T key, D val)
{
	DataLocation location = Search(key);
	if (location.found)
	{
		//exists already
		return;
	}
	stack<blockindex> path = location.path;
	bool added = false;
	blockindex index = 0;
	blockindex newblockindex = 0;
	while (!added && !path.empty())
	{
		index = path.top();
		path.pop();
		Node node;
		disk.Read(node, index);

		if (node.k < m) //still some space in current node
		{
			node.Insert(key, val, newblockindex);
			disk.Rewrite(node, index);
			added = true;
		}
		else // new brother with half of the values of the current node and shift m/2 up
		{
			Node brother;
			unsigned int middle = m / 2;
			T middleKey = node.keys[middle];
			D middleValue = node.data[middle];

			brother.k = node.k - middle - 1;
			for (auto i = middle + 1; i < node.k; i++)
			{
				brother.keys[i-middle - 1] = node.keys[i];
				brother.data[i - middle - 1] = node.data[i];
			}
			if(!node.isLeaf)
			{
				for (unsigned int i = middle + 1; i <= node.k; i++) {
					brother.index[i - middle - 1] = node.index[i];
				}
			}
			node.k = middle;
			if(key < node.keys[middle])
			{
				node.Insert(key, val, newblockindex);
			}
			else {
				brother.Insert(key, val, newblockindex);
			}
			key = middleKey;
			val = middleValue;

			disk.Rewrite(node, index);
			newblockindex = disk.Write(brother);
		}
	}
	//if we arrive at top but have a middle value that needs to be pushed up, we create a new root
	if(!added && path.empty())
	{
		Node root;
		root.isLeaf = false;
		root.keys[0] = key;
		root.data[0] = val;
		root.index[0] = index;
		root.index[1] = newblockindex;
		rootIndex = disk.Write(root);
	}
}

template <class T, class D, unsigned m>
DataLocation BTree<T, D, m>::Search(T key)
{
	Node node;
	blockindex index = rootIndex;
	DataLocation location;
	location.found = false;
	stack<blockindex> path;
	disk.Read(node, index);
	path.push(index);

	while (true)
	{
		unsigned int i = 0;
		while (i < node.k && node.keys[i] < key)
		{
			i++;
		}
		if (i < node.k)
		{
			if (key == node.keys[i])
			{
				location.blockindex_ = index;
				location.keyindex = i;
				location.found = true;
				location.path = path;
				return location;
			}
			if (node.isLeaf)
			{
				return location;
			}
			index = node.index[i - 1];
			path.push(index);
			disk.Read(node, index);
		}
		else
		{
			return location;
		}
	}
}

template <class T, class D, unsigned int m>
class BNode
{
private:
	friend class BTree<T, D, m>;
public:
	BNode(): k(0), isLeaf(true)
	{
	}

	const BNode<T, D, m>& operator=(const BNode<T, D, m>& b);

	int Insert(T key, D val, blockindex index);

	T keys[m];
	D data[m];
	blockindex index[m];
	/// <summary>
	/// The current number of keys
	/// </summary>
	unsigned int k;
	bool isLeaf;
};

template <class T, class D, unsigned m>
const BNode<T, D, m>& BNode<T, D, m>::operator=(const BNode<T, D, m>& b)
{
	k = b.k;
	isLeaf = b.isLeaf;
	for (unsigned int i = 0; i < k; i++)
	{
		keys[i] = b.keys[i];
		data[i] = b.data[i];
	}
	if (!isLeaf)
	{
		for (unsigned int i = 0; i <= k; i++)
		{
			index[i] = b.index[i];
		}
	}
	return *this;
}

template <class T, class D, unsigned m>
int BNode<T, D, m>::Insert(T key, D val, blockindex bindex)
{
	unsigned int j = 0;
	while (j < k && keys[j] < key) {
		j++;
	}

	// Schuif alles erachter 1 plaats op.
	for (unsigned int v = k; v > j; v--) {
		keys[v] = keys[v - 1];
		data[v] = data[v - 1];
		if (!isLeaf) {
			index[v + 1] = index[v];
		}
	}

	// Plaats de sl daar.
	keys[j] = key;
	data[j] = val;
	if (!isLeaf && bindex != 0) {
		index[j+1] = bindex;
	}
	k++;
	return j;
}
