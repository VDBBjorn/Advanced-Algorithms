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
	BLACK,
	RED
};

enum class Status
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
	RedBlackTree() : RedBlackTree<T, D>(Color::RED)
	{
	}

	RedBlackTree(Color c) : color(c), status(Status::UNKNOWN)
	{
	}

	RedBlackTree(Status s) : color(Color::RED), status(s)
	{
	}

	Color color;
	Status status;
	RedBlackTree<T, D>* Search(const T&) override;
	RedBlackTree<T, D>* Add(const T&, const D&) override;
	void Delete(const T&) override;
	RedBlackTree<T, D>* GetSibling();
	void Write(ostream& os) override;
};

template <class T, class D>
RedBlackTree<T, D>* RedBlackTree<T, D>::Search(const T& search)
{
	if (this->end() || this->get()->key == search)
	{
		this->status = Status::ROOT;
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
		tree->status = Status::ROOT;
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
		auto color = tree->color;
		if (tree->status == Status::ROOT)
		{
			node = new RedBlackNode<T, D>(key, data, new RedBlackTree<T, D>(Color::BLACK));
			color = Color::BLACK;
		}
		else
		{
			node = new RedBlackNode<T, D>(key, data, parent);
		}
		auto status = tree->status;
		*tree = static_cast<RedBlackTree<T, D>>(static_cast<unique_ptr<RedBlackNode<T, D>>>(node));
		tree->status = status;
		tree->color = color;
		parent = static_cast<RedBlackTree<T, D>*>(tree->get()->parent);

		bool doubleRed = (!parent->end() && parent->color == Color::RED);
		while (doubleRed)
		{
			TurnBlack(tree);

			assert(!tree->end());
			parent = static_cast<RedBlackTree<T, D>*>(tree->get()->parent);
			doubleRed = (tree->color == Color::RED && parent != nullptr && parent->color == Color::RED);
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
	if (static_cast<RedBlackTree<T, D>*>(grandParent->get()->left)->color == Color::RED && static_cast<RedBlackTree<T, D>*>(grandParent->get()->right)->color == Color::RED)
	{
		static_cast<RedBlackTree<T, D>*>(grandParent)->color = Color::RED;
		static_cast<RedBlackTree<T, D>*>(grandParent->get()->left)->color = Color::BLACK;
		static_cast<RedBlackTree<T, D>*>(grandParent->get()->right)->color = Color::BLACK;
	}
	TurnRootBlack(tree);
}

template <class T, class D>
void TurnRootBlack(RedBlackTree<T,D> * tree)
{
	RedBlackTree<T, D>* top = tree;
	RedBlackTree<T, D>* previous = nullptr;
	while (top->get() != nullptr)
	{
		previous = top;
		top = static_cast<RedBlackTree<T, D>*>(top->get()->parent);
	}
	previous->color = Color::BLACK;
	previous->status = Status::ROOT;
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
void RedBlackTree<T, D>::Write(ostream& os)
{
	if (this->get() != nullptr)
	{
		this->get()->left->Write(os);
		os << this->get()->key << " (" << static_cast<int>(this->status) << ") ";
		this->get()->right->Write(os);
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
	this->left = new RedBlackTree<T, D>(Status::LEFT);
	this->right = new RedBlackTree<T, D>(Status::RIGHT);
	this->parent = parent;
}
