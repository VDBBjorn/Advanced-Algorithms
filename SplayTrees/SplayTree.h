#pragma once
#include "../Tree/Tree.h"
#include <string>
#include <cassert>

enum class Status
{
	UNKNOWN = 1,
	ROOT = 2,
	LEFT = 3,
	RIGHT = 4
};

template <class T, class D>
class SplayTree;
template <class T, class D>
class SplayNode;

template <class T, class D>
bool isLeftChild(Tree<T, D> * child, Tree<T, D> * parent)
{
	if(parent->end())
	{
		return false;
	}
	if (parent->get()->left == child)
		return true;
	return false;
}

template <class T, class D>
class SplayTree : public Tree<T, D>
{
private:
	using Tree<T, D>::Tree;
public:
	SplayTree() : SplayTree<T, D>(Status::UNKNOWN)
	{
	}

	SplayTree(Status s) : status(s)
	{
	}

	Status status;

	SplayTree<T, D>* Search(const T&) override;
	SplayTree<T, D>* Search(const T&, SplayTree<T, D>*&);
	SplayTree<T, D>* Add(const T&, const D&) override;
	void Delete(const T&) override;

	void Rotate(bool left);
	SplayTree<T, D>* BottomUpSplay();
	void Write(ostream& os) override;
};

template <class T, class D>
SplayTree<T, D>* SplayTree<T, D>::Search(const T& search)
{
	/*if (!this->end() && !this->get()->parent->end())
	{
		return Search(search, static_cast<SplayTree<T, D>*>(this->get()->parent));
	}
	return Search(search, new SplayTree<T, D>());*/
	return nullptr;
}

template <class T, class D>
SplayTree<T, D>* SplayTree<T, D>::Search(const T& search, SplayTree<T, D>*& parent)
{
	if (this->end() || this->get()->key == search)
	{
		this->status = Status::ROOT;
		return this;
	}

	SplayTree<T, D>* tree = this;
	auto left = true;
	while (!tree->end() && tree->get()->key != search)
	{
		tree->get()->parent = parent;
		parent = tree;
		if (search < tree->get()->key)
		{
			tree = static_cast<SplayTree<T, D>*>(tree->get()->left);
			left = true;
		}
		else
		{
			tree = static_cast<SplayTree<T, D>*>(tree->get()->right);
			left = false;
		}
	}
	tree->status = (left ? Status::LEFT : Status::RIGHT);
	return tree;
}

template <class T, class D>
SplayTree<T, D>* SplayTree<T, D>::Add(const T& s, const D& d)
{
	SplayTree<T, D>* parent = new SplayTree<T, D>();
	SplayTree<T, D>* tree = Search(s, parent);
	auto node = new SplayNode<T, D>(s, d, parent);
	Status status = tree->status;
	*tree = static_cast<SplayTree<T, D>>(static_cast<unique_ptr<SplayNode<T, D>>>(node));
	tree->status = status;
	tree->BottomUpSplay();
	return tree;
}

template <class T, class D>
void SplayTree<T, D>::Delete(const T&)
{
	//TODO
}

/// <summary>
/// Rotates the tree to the left
/// </summary>
/// <param name="left">if set to <c>true</c> the tree gets rotated left. If set to <c>false</c> the tree gets rotated right.</param>
template <class T, class D>
void SplayTree<T, D>::Rotate(bool left)
{
	SplayTree<T, D> * q = new SplayTree<T, D>();
	if (left)
	{
		*q = static_cast<SplayTree<T, D>>(move(*(this->get()->right)));
		*(this->get()->right) = move(*(q->get()->left));
		*(q->get()->left) = move(*this);
	}
	else
	{
		*q = static_cast<SplayTree<T, D>>(move(*(this->get()->left)));
		*(this->get()->left) = move(*(q->get()->right));
		*(q->get()->right) = move(*this);
	}
	*this = move(*q);
}

template <class T, class D>
SplayTree<T, D>* SplayTree<T, D>::BottomUpSplay()
{
	SplayTree<T, D>* c = this;
	SplayTree<T, D>* parent = static_cast<SplayTree<T, D>*>(c->get()->parent);

	while (parent != nullptr) {
		//parent is root
		if (parent->end())
		{
			this->Rotate(!isLeftChild(this,parent));
			parent = nullptr;
		}
		//rotate intern
		else
		{
			SplayTree<T, D> * grandparent = static_cast<SplayTree<T, D>*>(parent->get()->parent);
			SplayTree<T, D> * grandparentHelper = nullptr;
			if(grandparent->end())
			{
				grandparentHelper = c;
			}
			else
			{
				if(isLeftChild(parent->get()->parent, grandparent))
				{
					grandparentHelper = static_cast<SplayTree<T, D>*>(grandparent->get()->left);
				}
				else
				{
					grandparentHelper = static_cast<SplayTree<T, D>*>(grandparent->get()->right);
				}
			}

			if(isLeftChild(parent,parent->get()->parent))
			{
				if(isLeftChild(c,parent))
				{
					grandparentHelper->Rotate(false);
				}
				else
				{
					static_cast<SplayTree<T, D>*>(parent->get()->parent->get()->left)->Rotate(true);
					grandparent->Rotate(false);
				}
			}
			c = grandparent;
			parent = static_cast<SplayTree<T, D>*>(grandparent->get()->parent);
		}
	}
	return c;
}

template <class T, class D>
void SplayTree<T, D>::Write(ostream& os)
{
	if (!this->end())
	{
		this->get()->left->Write(os);
		os << this->get()->key << " (" << static_cast<int>(this->status) << ") ";
		this->get()->right->Write(os);
	}
}

template <class T, class D>
class SplayNode : public Node<T, D>
{
private:
	using Node<T, D>::Node;
public:
	SplayNode(const T&, const D&);
	SplayNode(const T&, const D&, SplayTree<T, D>*);
};

template <class T, class D>
SplayNode<T, D>::SplayNode(const T& s, const D& d): SplayNode<T, D>(s, d, new SplayTree<T, D>())
{
}

template <class T, class D>
SplayNode<T, D>::SplayNode(const T& s, const D& d, SplayTree<T, D>* parent): Node<T, D>(s, d)
{
	this->parent = parent;
	this->left = new SplayTree<T, D>();
	this->right = new SplayTree<T, D>();
}
