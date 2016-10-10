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
using RedBlackNodePointer = std::unique_ptr<RedBlackNode<T, D>>;

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

	Color color;
	static RedBlackNode<T, D> * NullNodeImplementation;

	bool end();
	int depth();
	RedBlackTree<T, D> * ZoekSleutel(T);
	void Rotate(bool left);
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
	if (left) {

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
RedBlackTree<T, D>* RedBlackNode<T, D>::NullTreeImplementation = new NullNode<T, D>();


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