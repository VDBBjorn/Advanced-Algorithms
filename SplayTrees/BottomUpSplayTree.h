#pragma once
#include "../Tree/Tree.h"
#include <string>
#include <cassert>
#include "../SearchTrees/SearchTree.h"

template <class T, class D>
class BottomUpSplayTree;
template <class T, class D>
class SplayNode;

/// Name the unique pointer to a node
template <class T, class D>
using SplayNodePointer = unique_ptr<SplayNode<T, D>>;

template <class T, class D>
class BottomUpSplayTree : public SplayNodePointer<T, D>
{
public:
	BottomUpSplayTree()
	{
	}

	void Rotate(bool left);
	BottomUpSplayTree<T, D>* Search(const T&);
	BottomUpSplayTree<T, D>* Search(const T&, SplayNode<T, D>*&);
	BottomUpSplayTree<T, D>* Add(const T&, const D&);
	void Delete(const T&);
	void Write(ostream& os);
	BottomUpSplayTree<T, D>* BottomUpSplay();
};

template <class T, class D>
void BottomUpSplayTree<T, D>::Rotate(bool left)
{
	BottomUpSplayTree<T, D> tree;
	if (left)
	{
		tree = move((*this)->right);
		(*this)->right = move(tree->left);
		if ((*this)->right != nullptr)
		{
			(*this)->right->parent = (*this).get();
		}
		tree->parent = (*this)->parent;
		(*this)->parent = (*this)->left->parent;
		tree->left = move(*this);
		tree->left->parent = tree.get();
	}
	else
	{
		tree = move((*this)->left);
		(*this)->left = move(tree->right);
		if ((*this)->left != nullptr)
		{
			(*this)->left->parent = (*this).get();
		}
		tree->parent = (*this)->parent;
		tree->right = move(*this);
		tree->right->parent = tree.get();
	}
	*this = move(tree);
}

template <class T, class D>
BottomUpSplayTree<T, D>* BottomUpSplayTree<T, D>::Search(const T& search)
{
	SplayNode<T, D>* parent;
	return Search(search, parent);
}

template <class T, class D>
BottomUpSplayTree<T, D>* BottomUpSplayTree<T, D>::Search(const T& search, SplayNode<T, D>*& parent)
{
	if (this->End() || this->get()->key == search)
	{
		parent = nullptr;
		return this;
	}

	BottomUpSplayTree<T, D>* tree = this;
	while (!tree->End() && tree->get()->key != search)
	{
		if (tree->IsRoot())
		{
			tree->get()->parent = parent;
		}
		parent = tree->get();
		if (search < tree->get()->key)
		{
			tree = &(tree->get()->left);
		}
		else
		{
			tree = &(tree->get()->right);
		}
	}
	return tree;
}

template <class T, class D>
BottomUpSplayTree<T, D>* BottomUpSplayTree<T, D>::Add(const T& s, const D& d)
{
	BottomUpSplayTree<T, D>* parent = new BottomUpSplayTree<T, D>();
	BottomUpSplayTree<T, D>* tree = Search(s, parent);
	auto node = new SplayNode<T, D>(s, d, parent);
	*tree = BottomUpSplayTree<T,D>(node);
	tree->BottomUpSplay();
	return tree;
}

template <class T, class D>
void BottomUpSplayTree<T, D>::Delete(const T&)
{
	//TODO
}

template <class T, class D>
BottomUpSplayTree<T, D>* BottomUpSplayTree<T, D>::BottomUpSplay()
{
	BottomUpSplayTree<T, D>* c = this;
	BottomUpSplayTree<T, D>* parent = static_cast<BottomUpSplayTree<T, D>*>(c->parent);

	/*while (parent != nullptr) {
		//parent is root
		if (parent->End())
		{
			this->Rotate(!IsLeftChild(this,parent));
			parent = nullptr;
		}
		//rotate intern
		else
		{
			BottomUpSplayTree<T, D> * grandparent = static_cast<BottomUpSplayTree<T, D>*>(parent->get()->parent);
			BottomUpSplayTree<T, D> * grandparentHelper = nullptr;
			if(grandparent->End())
			{
				grandparentHelper = c;
			}
			else
			{
				if(IsLeftChild(parent->get()->parent, grandparent))
				{
					grandparentHelper = static_cast<BottomUpSplayTree<T, D>*>(grandparent->get()->left);
				}
				else
				{
					grandparentHelper = static_cast<BottomUpSplayTree<T, D>*>(grandparent->get()->right);
				}
			}

			if(IsLeftChild(parent,parent->get()->parent))
			{
				if(IsLeftChild(c,parent))
				{
					grandparentHelper->Rotate(false);
				}
				else
				{
					static_cast<BottomUpSplayTree<T, D>*>(parent->get()->parent->get()->left)->Rotate(true);
					grandparent->Rotate(false);
				}
			}
			c = grandparent;
			parent = static_cast<BottomUpSplayTree<T, D>*>(grandparent->get()->parent);
		}
	}*/
	return c;
}

template <class T, class D>
void BottomUpSplayTree<T, D>::Write(ostream& os)
{
	if (!this->End())
	{
		this->get()->left->Write(os);
		os << this->get()->key << " ";
		this->get()->right->Write(os);
	}
}

template <class T, class D>
class SplayNode
{
public:
	SplayNode() : parent(nullptr)
	{

	}

	SplayNode(const T& k, const D& d) : SplayNode(k, d, nullptr)
	{
	}

	SplayNode(const T& k, const D& d, SplayNode<T, D>* p) : key(k), data(d), parent(p)
	{
	}

	T key;
	D data;
	Node<T, D>* parent;
	BottomUpSplayTree<T, D> left;
	BottomUpSplayTree<T, D> right;
};
