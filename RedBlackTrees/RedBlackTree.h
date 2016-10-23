#pragma once
#include "../Tree/Tree.h"
#include <string>
#include <cassert>

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
class RedBlackTree : public Tree<T, D>
{
private:
	using Tree<T, D>::Tree;
public:
	RedBlackTree() : RedBlackTree<T, D>(Color::RED)
	{
	}

	RedBlackTree(Color c) : Tree(), color(c)
	{
	}

	Color color;
	RedBlackTree<T, D>* Search(const T&) override;
	RedBlackTree<T, D>* Search(const T&, RedBlackTree<T, D>*&);
	RedBlackTree<T, D>* Add(const T&, const D&) override;
	void Delete(const T&) override;
	bool IsRoot();
	RedBlackTree<T, D>* GetSibling();
	void Write(ostream& os) override;
	void Rotate(bool) override;
};

template <class T, class D>
RedBlackTree<T, D>* RedBlackTree<T, D>::Search(const T& search)
{
	RedBlackTree<T, D>* parent;
	return Search(search, parent);
}

template <class T, class D>
RedBlackTree<T, D>* RedBlackTree<T, D>::Search(const T& search, RedBlackTree<T, D>*& parent)
{
	if (this->End() || this->get()->key == search)
	{
		parent = new RedBlackTree<T, D>();
		return this;
	}

	RedBlackTree<T, D>* tree = this;
	while (!tree->End() && tree->get()->key != search)
	{
		if (tree->IsRoot()) {
			tree->parent = parent;
		}
		parent = tree;
		if (search < tree->get()->key)
		{
			tree = static_cast<RedBlackTree<T, D>*>(tree->get()->left);
		}
		else
		{
			tree = static_cast<RedBlackTree<T, D>*>(tree->get()->right);
		}
	}
	return tree;
}

template <class T, class D>
RedBlackTree<T, D>* RedBlackTree<T, D>::Add(const T& key, const D& data)
{
	RedBlackTree<T, D>* parent = new RedBlackTree<T, D>();
	RedBlackTree<T, D>* tree = Search(key, parent);


	if (tree->End())
	{
		RedBlackNode<T, D>* node;
		node = new RedBlackNode<T, D>(key, data);
		*tree = static_cast<RedBlackTree<T, D>>(static_cast<unique_ptr<RedBlackNode<T, D>>>(node));
		tree->parent = parent;
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
bool RedBlackTree<T, D>::IsRoot()
{
	return this->parent->End();
}

template <class T, class D>
RedBlackTree<T, D>* RedBlackTree<T, D>::GetSibling()
{
	if (this->parent->End())
	{
		return new RedBlackTree();
	}
	RedBlackTree<T, D>* parent = static_cast<RedBlackTree<T, D>*>(this->parent);
	if (this == parent->get()->left)
	{
		return static_cast<RedBlackTree<T, D>*>(parent->get()->right);
	}
	return static_cast<RedBlackTree<T, D>*>(parent->get()->left);
}


template <class T, class D>
void RedBlackTree<T, D>::Write(ostream& os)
{
	if (this->get() != nullptr)
	{
		this->get()->left->Write(os);
		os << this->get()->key;
		if (this->parent != nullptr && !this->parent->End())
		{
			os << " (" << this->parent->get()->key;
		}
		else
		{
			os << " (root";
		}
		os << ", " << colorstring[static_cast<int>(this->color)];
		os << ") ";
		this->get()->right->Write(os);
	}
}

template <class T, class D>
void RedBlackTree<T, D>::Rotate(bool left)
{
	RedBlackTree<T, D>* q = new RedBlackTree<T, D>();
	if (left)
	{
		*q = static_cast<RedBlackTree<T, D>>(move(*(this->get()->right)));
		*(this->get()->right) = move(*(q->get()->left));
		*(q->get()->left) = move(*this);
	}
	else
	{
		*q = static_cast<RedBlackTree<T, D>>(move(*(this->get()->left)));
		*(this->get()->left) = move(*(q->get()->right));
		q->get()->right = move(this);
	}
	*this = move(*q);
}

template <class T, class D>
class RedBlackNode : public Node<T, D>
{
private:
	using Node<T, D>::Node;
public:
	RedBlackNode(const T&, const D&);
};

template <class T, class D>
RedBlackNode<T, D>::RedBlackNode(const T& k, const D& d): Node<T, D>(k, d)
{
	this->left = new RedBlackTree<T, D>();
	this->right = new RedBlackTree<T, D>();
}
