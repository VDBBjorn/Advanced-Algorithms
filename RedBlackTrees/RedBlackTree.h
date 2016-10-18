#pragma once
#include "../Tree/Tree.h"
#include <string>
#include <cassert>

using namespace std;

template <class T, class D>
class RedBlackTree;
template <class T, class D>
class RedBlackNode;

enum Color
{
	BLACK,
	RED
};

enum Status
{
	UNKNOWN=1,
	ROOT=2,
	LEFT=3,
	RIGHT=4
};

template <class T, class D>
class RedBlackTree : public Tree<T, D>
{
private:
	using Tree<T, D>::Tree;
public:
	RedBlackTree() : RedBlackTree<T, D>(RED)
	{
	}

	RedBlackTree(Color c) : color(c), status(UNKNOWN)
	{
	}

	RedBlackTree(Status s) : color(RED), status(s)
	{
	}

	Color color;
	Status status;
	RedBlackTree<T, D>* Search(const T&) override;
	RedBlackTree<T, D>* Add(const T&, const D&) override;
	void Delete(const T&) override;
	RedBlackTree<T, D>* GetSibling();
	void Rotate(bool left);
	void write(ostream& os) override;
};

template <class T, class D>
RedBlackTree<T, D>* RedBlackTree<T, D>::Search(const T& search)
{
	if (this->end() || this->get()->key == search)
	{
		this->status = ROOT;
		return this;
	}

	RedBlackTree<T, D>* tree = this;
	RedBlackTree<T, D>* parent = new RedBlackTree<T, D>();
	while (!tree->end() && tree->get()->key != search)
	{
		tree->get()->parent = parent;
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
	RedBlackTree<T, D>* tree = this;
	RedBlackTree<T, D>* parent = new RedBlackTree<T, D>();
	if (tree->end() || tree->get()->key == key)
	{
		tree->status = ROOT;
	}
	else
	{
		while (!tree->end() && tree->get()->key != key)
		{
			tree->get()->parent = parent;
			parent = tree;
			if (key < tree->get()->key)
			{
				tree = static_cast<RedBlackTree<T, D>*>(tree->get()->left);
			}
			else
			{
				tree = static_cast<RedBlackTree<T, D>*>(tree->get()->right);
			}
		}
	}
	if (tree->end())
	{
		RedBlackNode<T, D>* node;
		if (tree->status == ROOT)
		{
			node = new RedBlackNode<T, D>(key, data, new RedBlackTree<T, D>(BLACK));
		}
		else
		{
			node = new RedBlackNode<T, D>(key, data, parent);
		}
		*tree = static_cast<RedBlackTree<T, D>>(static_cast<unique_ptr<RedBlackNode<T, D>>>(node));

		bool doubleRed = (!parent->end() && parent->color == RED);
		while (doubleRed)
		{
			TurnBlack(tree);

			assert(!tree->end());
			parent = static_cast<RedBlackTree<T, D>*>(tree->get()->parent);
			doubleRed = (tree->color == RED && parent != nullptr && parent->color == RED);
		}
	}
	return tree;
}

template <class T, class D>
void TurnBlack(RedBlackTree<T, D>* tree)
{
	RedBlackTree<T, D>* parent = static_cast<RedBlackTree<T, D>*>(tree->get()->parent);
	RedBlackTree<T, D>* grandParent = static_cast<RedBlackTree<T, D>*>(parent->get()->parent);

	// Simpele geval. De grootouder is zwart, de ouder en zijn broer rood.
	// Maak de grootouder rood, ouder en broer zwart. (zwart zakt 1 niveau).
	if (static_cast<RedBlackTree<T, D>*>(grandParent->get()->left)->color == RED && static_cast<RedBlackTree<T, D>*>(grandParent->get()->right)->color == RED)
	{
		static_cast<RedBlackTree<T, D>*>(grandParent)->color = RED;
		static_cast<RedBlackTree<T, D>*>(grandParent->get()->left)->color = BLACK;
		static_cast<RedBlackTree<T, D>*>(grandParent->get()->right)->color = BLACK;
	}
}

template <class T, class D>
void RedBlackTree<T, D>::Delete(const T&)
{
}

template <class T, class D>
RedBlackTree<T, D>* RedBlackTree<T, D>::GetSibling()
{
	if (this->get()->parent->end())
	{
		return new RedBlackTree();
	}
	RedBlackTree<T, D>* parent = static_cast<RedBlackTree<T, D>*>(this->get()->parent);
	if (this == parent->get()->left)
	{
		return static_cast<RedBlackTree<T, D>*>(parent->get()->right);
	}
	return static_cast<RedBlackTree<T, D>*>(parent->get()->left);
}

template <class T, class D>
void RedBlackTree<T, D>::Rotate(bool left)
{
	RedBlackTree<T, D> c;
	if (left)
	{
		c = move(this->get()->right);
		this->get()->right = move(c->left);
		if (this->get()->right != nullptr)
		{
			this->get()->right->parent = this->get();
			c->left = move(*this);
		}
	}
	else
	{
		c = move(this->get()->left);
		this->get()->left = move(c->right);
		if (this->get()->left != nullptr)
		{
			this->get()->left->parent = this->get();
			c->right = move(*this);
		}
		*this = move(c);
		this->get()->parent = this->get()->left->parent;
		this->get()->left->parent = this->get();
	}
}


template <class T, class D>
void RedBlackTree<T, D>::write(ostream& os)
{
	if (this->get() != nullptr)
	{
		this->get()->left->write(os);
		os << this->get()->key << " (" << static_cast<int>(this->status) << ") ";
		this->get()->right->write(os);
	}
}

template <class T, class D>
class RedBlackNode : public Node<T, D>
{
private:
	using Node<T, D>::Node;
public:
	RedBlackNode(const T&, const D&, RedBlackTree<T, D>*);
};

template <class T, class D>
RedBlackNode<T, D>::RedBlackNode(const T& k, const D& d, RedBlackTree<T, D>* parent): Node<T, D>(k, d)
{
	this->left = new RedBlackTree<T, D>(LEFT);
	this->right = new RedBlackTree<T, D>(RIGHT);
	this->parent = parent;
}
