#pragma once
#ifndef __QUAD_TREE_H
#define __QUAD_TREE_H
#include <cstdlib>
#include <iostream>
#include <queue>
#include <memory>

using namespace std;

struct Point
{
	int x;
	int y;
};

class Node;

class PointQuadTree : public unique_ptr<Node>
{
public:
	using unique_ptr<Node>::unique_ptr;

	PointQuadTree()
	{
	};
	PointQuadTree(Point p);

	PointQuadTree* Search(Point);
	void Add(Point);
	int Depth() const;
	void Write(ostream& os) const;

	friend ostream& operator<<(ostream& os, PointQuadTree& b)
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
		for (int i = 0; i < 4; i++)
		{
			children.resize(4);
		}
	}

	Node(Point k) : point(k)
	{
		children.resize(4);
	}

	friend class PointQuadTree;
private:
	Point point;
	vector<PointQuadTree> children;
};

inline PointQuadTree::PointQuadTree(Point p) : unique_ptr<Node>(new Node(p))
{
}

inline PointQuadTree* PointQuadTree::Search(Point k)
{
	PointQuadTree* loper = this;

	while (loper->get() != nullptr && (loper->get()->point.x != k.x && loper->get()->point.y != k.y))
	{
		if (k.x <= loper->get()->point.x)
		{
			if (k.y <= loper->get()->point.y)
			{//NW
				loper = &(loper->get()->children[0]);
			}
			else
			{//ZW
				loper = &(loper->get()->children[3]);
			}
		}
		else
		{
			if (k.y <= loper->get()->point.y)
			{//NO
				loper = &(loper->get()->children[1]);
			}
			else
			{//ZO
				loper = &(loper->get()->children[2]);
			}
		}
	}
	return loper;
}

inline void PointQuadTree::Add(Point k)
{
	PointQuadTree* plaats = Search(k);
	if (plaats->get() == nullptr)
	{
		*plaats = move(make_unique<Node>(k));
	}
}

inline int PointQuadTree::Depth() const
{
	int max_diepte = 0;
	for (int i = 0; i < 4; i++)
	{
		if (this->get()->children[i])
		{
			int kind_diepte = this->get()->children[i].Depth() + 1;
			if (kind_diepte > max_diepte)
			{
				max_diepte = kind_diepte;
			}
		}
	}
	return max_diepte;
}

inline void PointQuadTree::Write(ostream& os) const
{
	if (*this != nullptr)
	{
		os << this->get()->point.x << " " << this->get()->point.y << endl;
		for (int i = 0; i < 4; i++)
		{
			if (this->get()->children[i])
			{
				os << "kind: " << i << endl;
				this->get()->children[i].Write(os);
			}
		}
	}
}

#endif
