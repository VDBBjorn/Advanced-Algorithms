#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include "Point.h"

using namespace std;
class KDTree;
class Node;

enum class Dimension
{
	x = 0,
	y = 1
};

class KDTree : public unique_ptr<Node>
{
public:
	using unique_ptr<Node>::unique_ptr;

	KDTree()
	{
	}

	explicit KDTree(Point p);

	KDTree* Search(Point);
	KDTree* Add(Point);
	int Depth() const;
	void Write(ostream& os);

	friend ostream& operator<<(ostream& os, KDTree& b)
	{
		b.Write(os);
		return os;
	}

protected:
	KDTree* Search(Point, Node*&);
};

class Node
{
	friend class KDTree;
public:
	Node()
	{
	}

	Node(Point k) : point(k), dimension(Dimension::x)
	{
	}

	Node(Point k, Dimension d): point(k), dimension(d)
	{
	}

	Dimension GetNextDimension() const;
private:
	Point point;
	KDTree left, right;
	Dimension dimension;
};

inline KDTree::KDTree(Point p) : unique_ptr<Node>(new Node(p))
{
}

KDTree* KDTree::Search(Point k)
{
	Node* n = new Node();
	return Search(k, n);
}

inline Dimension Node::GetNextDimension() const
{
	if (this != nullptr && this->dimension == Dimension::x)
	{
		return Dimension::y;
	}
	return Dimension::x;
}

inline KDTree* KDTree::Add(Point k)
{
	Node* parent = nullptr;
	KDTree* place = Search(k, parent);
	if (place->get() == nullptr)
	{
		Node* node = new Node(k, parent->GetNextDimension());
		*place = unique_ptr<Node>(node);
	}
	return place;
}

int KDTree::Depth() const
{
	return 0;
}

void KDTree::Write(ostream& os)
{
	if (this->get() != nullptr)
	{
		this->get()->left.Write(os);
		os << this->get()->point << "\t" << static_cast<int>(this->get()->dimension) << endl;
		this->get()->right.Write(os);
	}
}

inline KDTree* KDTree::Search(Point p, Node*& parent)
{
	Dimension d = Dimension::x;
	KDTree* tree = this;

	while (tree->get() != nullptr && !(tree->get()->point == p)) {
		parent = tree->get();
		if (tree->get()->dimension == Dimension::x)
		{
			if (tree->get()->point.x < p.x)
			{
				tree = &(tree->get()->right);
			}
			else
			{
				tree = &(tree->get()->left);
			}
		}
		else
		{
			if (tree->get()->point.y < p.y)
			{
				tree = &(tree->get()->right);
			}
			else
			{
				tree = &(tree->get()->left);
			}
		}
	}
	return tree;
}
