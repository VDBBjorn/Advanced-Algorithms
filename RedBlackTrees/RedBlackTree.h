#pragma once
#include <iostream>
#include <queue>
#include <memory>

using namespace std;

enum Color
{
	RED,
	BLACK,
	DOUBLEBLACK
};

/// Definition of classes
template <class T, class D>
class RedBlackTree;
template <class T, class D>
class RedBlackNode;
template <class T, class D>
class NullTree;
template <class T, class D>
class NullNode;

/// Name the unique pointer to a node
template <class T, class D>
using RedBlackNodePointer = unique_ptr<RedBlackNode<T, D>>;

/// <summary>
/// Red-black tree
/// </summary>
template <class T, class D>
class RedBlackTree : public RedBlackNodePointer<T, D>
{
	using RedBlackNodePointer<T, D>::RedBlackNodePointer;
	friend class RedBlackNode<T, D>;

public:
	RedBlackTree() : RedBlackTree<T, D>(NullNodeImplementation)
	{

	}

/*	RedBlackTree<T, D>(RedBlackTree<T, D>&& b) : unique_ptr<RedBlackNode<T, D>>(move(b)) {
	}

	RedBlackTree<T, D>& operator=(RedBlackTree<T, D>&& b) {
		unique_ptr<RedBlackNode<T, D>>::operator=(move(b));
		return *this;
	}*/

	Color color;
	static RedBlackNode<T, D> * NullNodeImplementation;

	bool end();
	int depth();
	RedBlackTree<T, D> * ZoekSleutel(T);
	void Rotate(bool);
	void AddBottomUp(T, D);
};

template<class T, class D>
RedBlackNode<T, D>* RedBlackTree<T, D>::NullNodeImplementation = new NullNode<T, D>();

template <class T, class D>
bool RedBlackTree<T, D>::end()
{
	return this->get() == NullNodeImplementation;
}


template <class T, class D>
int RedBlackTree<T, D>::depth()
{
	if (end())
	{
		return 0;
	}
	return max(this->get()->left->depth(), this->get()->right->depth()) + 1;
}

template<class T, class D>
RedBlackTree<T, D> * RedBlackTree<T, D>::ZoekSleutel(T zoek) {
	if (end() || this->get()->data == zoek) {
		return this;
	}
	if (zoek < this->get()->data) {
		return this->get()->left->ZoekSleutel(zoek);
	}
	return this->get()->right->ZoekSleutel(zoek);
}

template<class T, class D>
void RedBlackTree<T, D>::Rotate(bool left) {
	RedBlackTree<T, D> c;
	if (left) {
		c = move(this->get()->right);
		this->get()->right = move(c->left);
		if(this->get()->right != nullptr)
		{
			this->get()->right->parent = this->get();
			c->left = move(*this);
		}
	}
	else
	{
		c = move(this->get()->left);
		this->get()->left = move(c->right);
		if(this->get()->left != nullptr)
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
void RedBlackTree<T, D>::AddBottomUp(T key, D data)
{
	if(this->get() == NullNodeImplementation)
	{
		cout << "leeg";
		auto node = unique_ptr<RedBlackNode<T, D>>(new RedBlackNode<T, D>(key, data));
		node->parent = nullptr;
		(*this) = move(node);

	}
	else
	{
		cout << "niet leeg";
	}
}

/// <summary>
/// Red-black node
/// </summary>
template <class T, class D>
class RedBlackNode
{
	friend class RedBlackTree<T, D>;
public:
	RedBlackNode(Color c, RedBlackTree<T, D>* p) : color(c), parent(p)
	{
	}

	RedBlackNode(const T& k, const D& d) : key(k), data(d), color(RED), parent(nullptr), left(NullTreeImplementation), right(NullTreeImplementation)
	{
	}

	T key;
	D data;
	Color color;

	static RedBlackTree<T, D>* NullTreeImplementation;

	RedBlackTree<T, D>* parent;
	RedBlackTree<T, D>* left;
	RedBlackTree<T, D>* right;
};

template <class T, class D>
RedBlackTree<T, D>* RedBlackNode<T, D>::NullTreeImplementation = new NullTree<T, D>();


/// <summary>
/// Representation of a null-tree
/// </summary>
template <class T, class D>
class NullTree : public RedBlackTree<T, D>
{
public:
	NullTree<T, D>() : RedBlackTree<T, D>() {}
	NullTree<T, D>(RedBlackTree<T, D>&& tree) : RedBlackTree<T, D>(move(tree))
	{
		RedBlackTree<T, D>::color = BLACK;
	}
};

/// <summary>
/// Representation of a null-node
/// </summary>
template <class T, class D>
class NullNode : public RedBlackNode<T, D>
{
public:
	NullNode()
		: RedBlackNode<T, D>(BLACK, nullptr)
	{
	}
};