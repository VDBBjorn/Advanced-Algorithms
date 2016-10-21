#pragma once
#include "../Tree/Tree.h"

using namespace std;

template <class T, class D>
class SearchTree;
template <class T, class D>
class SearchNode;

template <class T, class D>
class SearchTree : public Tree<T, D>
{
private:
	using Tree<T, D>::Tree;
public:
	SearchTree()
	{
	}

	SearchTree<T, D>* Search(const T&) override;
	SearchTree<T, D>* Add(const T&, const D&) override;
	void Delete(const T&) override;
	void Rotate(bool) override;
};

template <class T, class D>
SearchTree<T, D>* SearchTree<T, D>::Search(const T& search)
{
	if (this->end() || this->get()->key == search)
	{
		return this;
	}
	if (search < this->get()->key)
	{
		return static_cast<SearchTree<T, D>*>(this->get()->left->Search(search));
	}
	return static_cast<SearchTree<T, D>*>(this->get()->right->Search(search));
}

template <class T, class D>
SearchTree<T, D>* SearchTree<T, D>::Add(const T& key, const D& data)
{
	SearchTree<T, D>* tree = Search(key);
	if (tree->end())
	{
		*tree = static_cast<SearchTree<T, D>>(static_cast<unique_ptr<SearchNode<T, D>>>(new SearchNode<T, D>(key, data)));
	}
	return tree;
}

template <class T, class D>
void SearchTree<T, D>::Delete(const T& key)
{
	//TODO
}

/// <summary>
/// Rotates the tree to the left, omitting parent pointers (as they are not used in searchtrees)
/// </summary>
/// <param name="left">if set to <c>true</c> the tree gets rotated left. If set to <c>false</c> the tree gets rotated right.</param>
template <class T, class D>
void SearchTree<T, D>::Rotate(bool left)
{
	SearchTree<T, D> * q = new SearchTree<T,D>();
	if (left)
	{
		*q = static_cast<SearchTree<T,D>>(move(*(this->get()->right)));
		*(this->get()->right) = move(*(q->get()->left));
		*(q->get()->left) = move(*this);
	}
	else
	{
		*q = static_cast<SearchTree<T, D>>(move(*(this->get()->left)));
		*(this->get()->left) = move(*(q->get()->right));
		*(q->get()->right) = move(*this);
	}
	*this = move(*q);
}

template <class T, class D>
class SearchNode : public Node<T, D>
{
private:
	using Node<T, D>::Node;
public:
	SearchNode(const T& k, const D& d);
};

template <class T, class D>
SearchNode<T, D>::SearchNode(const T& k, const D& d): Node<T, D>(k, d)
{
	this->left = new SearchTree<T, D>();
	this->right = new SearchTree<T, D>();
	this->parent = new SearchTree<T, D>();
}
