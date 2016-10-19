#pragma once
#include "../Tree/Tree.h"
#include <string>
#include <cassert>

template <class T, class D>
class SplayTree;
template <class T, class D>
class SplayNode;

template <class T, class D>
class SplayTree : public Tree<T, D>
{
private:
	using Tree<T, D>::Tree;
public:
	SplayTree<T, D>* Search(const T&) override;
	SplayTree<T, D>* Add(const T&, const D&) override;
	void Delete(const T&) override;

	void Rotate(bool left);
};

template <class T, class D>
SplayTree<T, D>* SplayTree<T, D>::Search(const T& search)
{
	if (this->end() || this->get()->key == search)
	{
		return this;
	}
	if (search < this->get()->key)
	{
		return static_cast<SplayTree<T, D>*>(this->get()->left->Search(search));
	}
	return static_cast<SplayTree<T, D>*>(this->get()->right->Search(search));
}

template <class T, class D>
SplayTree<T, D>* SplayTree<T, D>::Add(const T&, const D&)
{
	return nullptr;
}

template <class T, class D>
void SplayTree<T, D>::Delete(const T&)
{
}

template <class T, class D>
void SplayTree<T, D>::Rotate(bool left)
{
	SplayTree<T, D> c;
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
class SplayNode : public Node<T, D>
{
private:
	using Node<T, D>::Node;
public:
	SplayNode(const T&, const D&, SplayTree<T, D>*);
};

template <class T, class D>
SplayNode<T, D>::SplayNode(const T& s, const D& d, SplayTree<T, D>* parent): Node<T,D>(s,d)
{
	this->parent = parent;
}
