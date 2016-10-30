#pragma once
#include <cstdlib>
#include <iostream>
#include <queue>
#include <memory>
#include "Point.h"

using namespace std;

class Node;

class KDTree : public unique_ptr<Node>
{
public:
	using unique_ptr<Node>::unique_ptr;

	KDTree()
	{
	}

	explicit KDTree(Point p);

	KDTree* Search(Point);
	void Add(Point);
	int Depth() const;
	void Write(ostream& os) const;

	friend ostream& operator<<(ostream& os, KDTree& b)
	{
		b.Write(os);
		return os;
	}
};

class Node
{
public:
	Node()
	{
	}

	Node(Point k) : point(k)
	{
	}

	friend class KDTree;
private:
	Point point;
	KDTree left, right;
};

KDTree::KDTree(Point p) : unique_ptr<Node>(new Node(p))
{
}

KDTree* KDTree::Search(Point k)
{

}

void KDTree::Add(Point k)
{
}

int KDTree::Depth() const
{
}

void KDTree::Write(ostream& os) const
{
}
