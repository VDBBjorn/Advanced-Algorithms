#pragma once
#include <cstdlib>
#include <iostream>
#include <memory>
#include "Point.h"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

using namespace std;
class KDTree;
class Node;

enum class Dimension
{
	x = 0,
	y = 1
};

inline void Split(const string& s, char delim, vector<string>& elems)
{
	stringstream ss;
	ss.str(s);
	string item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}
}

inline vector<string> Split(const string& s, char delim)
{
	vector<string> elems;
	Split(s, delim, elems);
	return elems;
}


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
	void Write(ostream& os) const;
	bool Exists(Point);
	pair<unsigned, KDTree*> GetClosest(Point, int& visited);
	bool Read(string filename);

	friend ostream& operator<<(ostream& os, KDTree& b)
	{
		b.Write(os);
		return os;
	}

protected:
	KDTree* Search(Point, Node*&);
	KDTree* SearchNext(Point);
	KDTree* SearchNext(Point p, Dimension d);
	bool GetClosestDFS(Point, pair<unsigned, KDTree*>&, int& visited, Dimension d);
};

class Node
{
	friend class KDTree;
public:
	Node(): dimension(Dimension::x)
	{
	}

	explicit Node(Point k) : point(k), dimension(Dimension::x)
	{
	}

	Node(Point k, Dimension d): point(k), dimension(d)
	{
	}

	Dimension GetNextDimension() const;
public:
	Point point;
	KDTree left, right;
	Dimension dimension;
};

inline KDTree::KDTree(Point p) : unique_ptr<Node>(new Node(p))
{
}

inline KDTree* KDTree::Search(Point k)
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

inline void KDTree::Write(ostream& os) const
{
	if (this->get() != nullptr)
	{
		this->get()->left.Write(os);
		os << this->get()->point << "\t" << static_cast<int>(this->get()->dimension) << endl;
		this->get()->right.Write(os);
	}
}

inline bool KDTree::Exists(Point p)
{
	KDTree* place = Search(p);
	if (place->get() == nullptr)
	{
		return false;
	}
	return true;
}

inline Dimension GetNextDimension(Dimension d)
{
	if (d == Dimension::x)
	{
		return Dimension::y;
	}
	return Dimension::x;
}

inline bool KDTree::GetClosestDFS(Point p, pair<unsigned int, KDTree*>& best, int& visited, Dimension d)
{
	if (this->get() == nullptr) return false;;
	visited++;
	auto distance = (*this)->point.QuadraticDistance(p);
	bool returnPrune = false;
	if (distance <= best.first)
	{
		best = make_pair((*this)->point.QuadraticDistance(p), this); 
		returnPrune = true;
	}
	if (d == Dimension::x)
	{
		if ((*this)->point.x < p.x)
		{
			bool prune = (*this)->right.GetClosestDFS(p, best, visited, GetNextDimension(d));
			if (!prune)
			{
				(*this)->left.GetClosestDFS(p, best, visited, GetNextDimension(d));
			}
		}
		else
		{
			bool prune = (*this)->left.GetClosestDFS(p, best, visited, GetNextDimension(d));
			if (!prune)
			{
				(*this)->right.GetClosestDFS(p, best, visited, GetNextDimension(d));
			}
		}
	}
	else
	{
		if ((*this)->point.y < p.y)
		{
			bool prune = (*this)->right.GetClosestDFS(p, best, visited, GetNextDimension(d));
			if (!prune)
			{
				(*this)->left.GetClosestDFS(p, best, visited, GetNextDimension(d));
			}
		}
		else
		{
			bool prune = (*this)->left.GetClosestDFS(p, best, visited, GetNextDimension(d));
			if (!prune)
			{
				(*this)->right.GetClosestDFS(p, best, visited, GetNextDimension(d));
			}
		}
	}
	return returnPrune;
}

inline pair<unsigned, KDTree*> KDTree::GetClosest(Point p, int& visited)
{
	visited++;
	Dimension d = Dimension::x;
	KDTree* tree = this;
	pair<unsigned int, KDTree*> best = make_pair(tree->get()->point.QuadraticDistance(p), tree);
	if (tree->get() != nullptr)
	{
		tree->GetClosestDFS(p, best, visited, d);
	}
	return best;
}

inline bool KDTree::Read(string filename)
{
	ifstream input;
	input.open(filename);
	if (input.is_open())
	{
		while (!input.eof())
		{
			int x, y;
			input >> x;
			input >> y;
			this->Add(Point(x, y));
		}
		return true;
	}
	return false;
}

inline KDTree* KDTree::Search(Point p, Node*& parent)
{
	Dimension d = Dimension::x;
	KDTree* tree = this;

	while (tree->get() != nullptr && !(tree->get()->point == p))
	{
		parent = tree->get();
		tree = tree->SearchNext(p);
	}
	return tree;
}

inline KDTree* KDTree::SearchNext(Point p)
{
	return SearchNext(p, (*this)->dimension);
}

inline KDTree* KDTree::SearchNext(Point p, Dimension d)
{
	auto tree = this;
	if (d == Dimension::x)
	{
		if (tree->get()->point.x <= p.x)
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
		if (tree->get()->point.y <= p.y)
		{
			tree = &(tree->get()->right);
		}
		else
		{
			tree = &(tree->get()->left);
		}
	}
	return tree;
}
