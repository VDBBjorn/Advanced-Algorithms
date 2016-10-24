#pragma once
#include "../Tree/Tree.h"
#include <string>
#include <cassert>
#include "../SearchTrees/SearchTree.h"

using namespace std;

template <class T, class D>
class RedBlackTree;
template <class T, class D>
class RedBlackNode;

enum class Color
{
	BLACK = 0,
	RED = 1
};

const string colorstring[] = {"B", "R"};

template <class T, class D>
class RedBlackTree : public SearchTree<T, D>
{
private:
	using SearchTree<T, D>::SearchTree;
public:
	RedBlackTree() : RedBlackTree<T, D>(Color::RED)
	{
	}

	RedBlackTree(Color c) : SearchTree(), color(c)
	{
	}

	Color color;
	RedBlackTree<T, D>* Search(const T&) override;
	RedBlackTree<T, D>* Search(const T&, RedBlackNode<T, D>*&);
	RedBlackTree<T, D>* Add(const T&, const D&) override;
	void Delete(const T&) override;
	bool IsLeftChild();
	bool IsRoot();
	void Write(ostream& os) override; 
	friend ostream& operator<<(ostream& os, RedBlackTree<T, D>& t)
	{
		t.Write(os);
		return os;
	}
};

template <class T, class D>
RedBlackTree<T, D>* RedBlackTree<T, D>::Search(const T& search)
{
	RedBlackNode<T, D>* parent;
	return Search(search, parent);
}

template <class T, class D>
RedBlackTree<T, D>* RedBlackTree<T, D>::Search(const T& search, RedBlackNode<T, D>*& parent)
{
	if (this->End() || this->get()->key == search)
	{
		parent = nullptr;
		return this;
	}

	RedBlackTree<T, D>* tree = this;
	while (!tree->End() && tree->get()->key != search)
	{
		if (tree->IsRoot())
		{
			tree->get()->parent = parent;
		}
		parent = static_cast<RedBlackNode<T, D>*>(tree->get());
		if (search < tree->get()->key)
		{
			tree = static_cast<RedBlackTree<T, D>*>(&(tree->get()->left));
		}
		else
		{
			tree = static_cast<RedBlackTree<T, D>*>(&(tree->get()->right));
		}
	}
	return tree;
}

template <class T, class D>
RedBlackTree<T, D>* RedBlackTree<T, D>::Add(const T& key, const D& data)
{
	RedBlackNode<T, D>* parent = new RedBlackNode<T, D>();
	RedBlackTree<T, D>* tree = Search(key, parent);


	if (tree->End())
	{
		RedBlackNode<T, D>* node;
		node = new RedBlackNode<T, D>(key, data);
		*tree = static_cast<RedBlackTree<T, D>>(static_cast<unique_ptr<RedBlackNode<T, D>>>(node));
		tree->get()->parent = parent;
		tree->color = Color::RED;

		TurnBlack(tree);
	}
	return tree;
}

template <class T, class D>
void TurnBlack(RedBlackTree<T, D>*& tree)
{
	// root should be black
	if (tree->IsRoot())
	{
		tree->color = Color::BLACK;
		return;
	}
}

/*
RedBlackTree<T, D>* parent = static_cast<RedBlackTree<T, D>*>(tree->parent);

if (parent->End() || parent->parent == nullptr)
return;
RedBlackTree<T, D>* grandParent = static_cast<RedBlackTree<T, D>*>(parent->parent);

// Simpele geval. De grootouder is zwart, de ouder en zijn broer rood.
// Maak de grootouder rood, ouder en broer zwart. (zwart zakt 1 niveau).
if (static_cast<RedBlackTree<T, D>*>(grandParent->get()->left)->color == Color::RED && static_cast<RedBlackTree<T, D>*>(grandParent->get()->right)->color == Color::RED)
{
	grandParent->color = Color::RED;
	static_cast<RedBlackTree<T, D>*>(grandParent->get()->left)->color = Color::BLACK;
	static_cast<RedBlackTree<T, D>*>(grandParent->get()->right)->color = Color::BLACK;
}
else
{
	bool parentLeft = parent->IsLeftChild();
	bool childLeft = tree->IsLeftChild();
	// Geval: kind en ouder liggen niet op 1 lijn
	if (childLeft != parentLeft)
	{
		if (parentLeft)
			parent->get()->left->Rotate(true);
		else
			parent->get()->right->Rotate(false);
	}

	// Geval: kind en ouder liggen op 1 lijn
	// Roteer ouder en grootouder
	grandParent->color = Color::RED;
	parent->color = Color::BLACK;
	grandParent->Rotate(!parentLeft);
}
*/

template <class T, class D>
void RedBlackTree<T, D>::Delete(const T&)
{
}

template <class T, class D>
bool RedBlackTree<T, D>::IsLeftChild()
{
	if (this->get()->parent == nullptr)
	{
		return false;
	}
	if (this->parent->get()->left == this)
		return true;
	return false;
}

template <class T, class D>
bool RedBlackTree<T, D>::IsRoot()
{
	return this->get()->parent == nullptr;
}


template <class T, class D>
void RedBlackTree<T, D>::Write(ostream& os)
{
	if (this->get() != nullptr)
	{
		this->get()->left.Write(os);
		os << this->get()->key;
		os << " (";
		/*if (this->parent != nullptr && !this->parent->End())
		{
			os << this->parent->get()->key;
		}
		else
		{
			os << " (root";
		}*/
		os << ", " << colorstring[static_cast<int>(this->color)];
		os << ") ";
		this->get()->right.Write(os);
	}
}

template <class T, class D>
class RedBlackNode : public SearchNode<T, D>
{
private:
	using SearchNode<T, D>::SearchNode;
public:
	RedBlackNode(): SearchNode()
	{
	}

	RedBlackNode(const T&, const D&);
};

template <class T, class D>
RedBlackNode<T, D>::RedBlackNode(const T& k, const D& d): SearchNode<T, D>(k, d)
{
}
