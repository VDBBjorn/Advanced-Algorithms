#pragma once
#include "../Tree/Tree.h"
#include <string>
#include <cassert>
#include "../SearchTrees/SearchTree.h"

template <class T, class D>
class SplayTree;
template <class T, class D>
class SplayNode;

template <class T, class D>
class SplayTree : public SearchTree<T, D>
{
private:
	using SearchTree<T, D>::SearchTree;
public:
	SplayTree()
	{
	}

	SplayTree<T, D>* Search(const T&) override;
	SplayTree<T, D>* Search(const T&, SplayNode<T, D>*&);
	SplayTree<T, D>* Add(const T&, const D&) override;
	void Delete(const T&) override;
	void Write(ostream& os) override;
	SplayTree<T, D>* BottomUpSplay();
};

template <class T, class D>
SplayTree<T, D>* SplayTree<T, D>::Search(const T& search)
{
	SplayNode<T, D>* parent;
	return Search(search, parent);
}

template <class T, class D>
SplayTree<T, D>* SplayTree<T, D>::Search(const T& search, SplayNode<T, D>*& parent)
{
	if (this->End() || this->get()->key == search)
	{
		parent = nullptr;
		return this;
	}

	SplayTree<T, D>* tree = this;
	while (!tree->End() && tree->get()->key != search)
	{
		if (tree->IsRoot())
		{
			tree->get()->parent = parent;
		}
		parent = static_cast<SplayTree<T, D>*>(tree->get());
		if (search < tree->get()->key)
		{
			tree = static_cast<SplayTree<T, D>*>(&(tree->get()->left));
		}
		else
		{
			tree = static_cast<SplayTree<T, D>*>(&(tree->get()->right));
		}
	}
	return tree;
}

template <class T, class D>
SplayTree<T, D>* SplayTree<T, D>::Add(const T& s, const D& d)
{
	SplayTree<T, D>* parent = new SplayTree<T, D>();
	SplayTree<T, D>* tree = Search(s, parent);
	auto node = new SplayNode<T, D>(s, d, parent);
	*tree = static_cast<SplayTree<T, D>>(static_cast<unique_ptr<SplayNode<T, D>>>(node));
	tree->BottomUpSplay();
	return tree;
}

template <class T, class D>
void SplayTree<T, D>::Delete(const T&)
{
	//TODO
}

template <class T, class D>
SplayTree<T, D>* SplayTree<T, D>::BottomUpSplay()
{
	SplayTree<T, D>* c = this;
	SplayTree<T, D>* parent = static_cast<SplayTree<T, D>*>(c->parent);
	
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
			SplayTree<T, D> * grandparent = static_cast<SplayTree<T, D>*>(parent->get()->parent);
			SplayTree<T, D> * grandparentHelper = nullptr;
			if(grandparent->End())
			{
				grandparentHelper = c;
			}
			else
			{
				if(IsLeftChild(parent->get()->parent, grandparent))
				{
					grandparentHelper = static_cast<SplayTree<T, D>*>(grandparent->get()->left);
				}
				else
				{
					grandparentHelper = static_cast<SplayTree<T, D>*>(grandparent->get()->right);
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
					static_cast<SplayTree<T, D>*>(parent->get()->parent->get()->left)->Rotate(true);
					grandparent->Rotate(false);
				}
			}
			c = grandparent;
			parent = static_cast<SplayTree<T, D>*>(grandparent->get()->parent);
		}
	}*/
	return c;
}

template <class T, class D>
void SplayTree<T, D>::Write(ostream& os)
{
	if (!this->End())
	{
		this->get()->left->Write(os);
		os << this->get()->key << " ";
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

	SplayNode<T, D> * parent;
};

template <class T, class D>
SplayNode<T, D>::SplayNode(const T& s, const D& d): SplayNode<T, D>(s, d, new SplayTree<T, D>())
{
}

template <class T, class D>
SplayNode<T, D>::SplayNode(const T& s, const D& d, SplayTree<T, D>* parent): Node<T, D>(s, d)
{
	this->parent = parent;
}
