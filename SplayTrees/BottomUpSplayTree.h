#pragma once
#include "../Tree/Tree.h"
#include <string>
#include <cassert>

template <class T, class D>
class BottomUpSplayTree;
template <class T, class D>
class SplayNode;
template <class T, class D>
class RootSplayNode;

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

	explicit BottomUpSplayTree(SplayNode<T, D>*);

	void Rotate(bool left);
	BottomUpSplayTree<T, D>* Search(const T&);
	BottomUpSplayTree<T, D>* Search(const T&, SplayNode<T, D>*&);
	BottomUpSplayTree<T, D>* Add(const T&, const D&);
	void Delete(const T&);
	void Write(ostream& os);
	BottomUpSplayTree<T, D>* BottomUpSplay();
	int Depth();
	bool End();
	BottomUpSplayTree<T, D>* GetParentTree();
};

template <class T, class D>
BottomUpSplayTree<T, D>::BottomUpSplayTree(SplayNode<T, D>* node)
{
	this->reset(std::move(node));
}

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
		if ((*tree)->parent == nullptr)
		{
			tree->get()->parent = nullptr;
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

	//tree->BottomUpSplay();

	return tree;
}

template <class T, class D>
BottomUpSplayTree<T, D>* BottomUpSplayTree<T, D>::Add(const T& s, const D& d)
{
	SplayNode<T, D>* parent = new SplayNode<T, D>();
	BottomUpSplayTree<T, D>* tree = Search(s, parent);
	if (tree->End())
	{
		auto node = new SplayNode<T, D>(s, d, parent);
		*tree = BottomUpSplayTree<T, D>(node);
		tree->BottomUpSplay();
	}
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
	BottomUpSplayTree<T, D>* parent = c->GetParentTree();
	BottomUpSplayTree<T, D>* grandParent = parent->GetParentTree();

	while (parent != nullptr)
	{
		// ZIG
		// Er is geen grootouder meer -> roteer over root
		if (grandParent == nullptr)
		{
			root->Rotate(false);
		}
		// Er is nog een grootouder
		else
		{
			bool childLeft = ((*parent)->left == *c ? true : false);
			bool parentLeft = ((*grandParent)->left == *parent ? true : false);
			// als c en p op dezelfde lijn liggen
			// ZIG ZIG
			if (childLeft == parentLeft)
			{
				grandParent->Rotate(false);
				parent->Rotate(false);
			}
			// als c en p niet op dezelfde lijn liggen
			// ZIG ZAG
			else
			{
				parent->Rotate(true);
				grandParent->Rotate(false);
			}
		}
		parent = c->GetParentTree();
		grandParent = parent->GetParentTree();
	}
	return c;
}

template <class T, class D>
bool BottomUpSplayTree<T, D>::End()
{
	return this->get() == nullptr;
}

template <class T, class D>
BottomUpSplayTree<T, D>* BottomUpSplayTree<T, D>::GetParentTree()
{
	if(this != nullptr && this->get() != nullptr)
		return this->get()->GetParentTree();
	return nullptr;
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
int BottomUpSplayTree<T, D>::Depth()
{
	if (End())
	{
		return 0;
	}
	return max((*this)->left.Depth(), (*this)->right.Depth()) + 1;
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
	SplayNode<T, D>* parent;
	BottomUpSplayTree<T, D> left;
	BottomUpSplayTree<T, D> right;

	BottomUpSplayTree<T, D>* GetParentTree();
};

template <class T, class D>
class RootSplayNode: public SplayNode<T, D>
{
public:
	RootSplayNode()
	{
		
	}

	BottomUpSplayTree<T, D>* root;
};


template <class T, class D>
BottomUpSplayTree<T, D>* SplayNode<T, D>::GetParentTree()
{
	if (parent != nullptr)
	{
		if (parent->parent != nullptr) {
			if (parent->parent->left.get() == parent)
			{
				return &(parent->parent->left);
			}
			else
			{
				return &(parent->parent->right);
			}
		}
		else
		{
			// root is parent
			return ;
		}
	}
	return nullptr;
}
