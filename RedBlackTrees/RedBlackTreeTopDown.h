#pragma once
#include <string>
#include <cassert>
#include <memory>

using namespace std;

template <class T, class D>
class RedBlackTreeTopDown;
template <class T, class D>
class RBNode;

/// Name the unique pointer to a node
template <class T, class D>
using RBNodePointer = unique_ptr<RBNode<T, D>>;

enum class Color
{
	BLACK = 0,
	RED = 1,
	DOUBLEBLACK = 2
};

string colorstring[] = { "B", "R", "BB" };

template <class T, class D>
class RedBlackTreeTopDown : public RBNodePointer<T, D>
{
	using RBNodePointer<T, D>::RBNodePointer;
	friend class RBNode<T, D>;
public:
	RedBlackTreeTopDown()
	{
	}

	explicit RedBlackTreeTopDown(RBNode<T, D>*);
	RedBlackTreeTopDown<T, D>* Search(const T& search);
	RedBlackTreeTopDown<T, D>* Add(const T&, const D&);
	void Delete(const T&);

	void Rotate(bool);

	bool IsLeftChild();
	bool IsRoot();
	bool End();
	int Depth();
	int NumberOfNodes();
	RBNode<T, D>* GetParent();
	RedBlackTreeTopDown<T, D>& GetSibling();
	RedBlackTreeTopDown<T, D>& GetUnderPredecessor();

	void Write(ostream& os);

	friend ostream& operator<<(ostream& os, RedBlackTreeTopDown<T, D>& t)
	{
		t.Write(os);
		return os;
	}

protected:
	RedBlackTreeTopDown<T, D>* Search(const T&, RBNode<T, D>*&);
};

template <class T, class D>
RedBlackTreeTopDown<T, D>::RedBlackTreeTopDown(RBNode<T, D>* node)
{
	this->reset(std::move(node));
}

template <class T, class D>
RedBlackTreeTopDown<T, D>* RedBlackTreeTopDown<T, D>::Search(const T& search)
{
	if (this->End())
	{
		return this;
	}
	RBNode<T, D>* parent = this->get()->parent;
	return Search(search, parent);
}

/// Op de weg naar beneden mogen we geen rode broers toelaten
/// Wanneer we dus voorbij een zwarte knoop met twee rode kinderen komen, dan maken we die knoop rood en zijn kinderen zwart.
/// Tijdens de afdaling moeten er dus steeds (wijzers naar) vier opeenvolgende knopen bijgehouden worden
/// om twee opeenvolgende rode knopen (ouder en kind) te elimineren moeten ouder en grootouder roteren, waarbij ook de overgrootouder moet aangepast worden (als die bestaat).
template <class T, class D>
RedBlackTreeTopDown<T, D>* RedBlackTreeTopDown<T, D>::Add(const T& key, const D& data)
{
	RedBlackTreeTopDown<T, D> * tree = this;
	RBNode<T, D>* parent = (*this)->parent;
	RBNode<T, D>* grandParent = nullptr;
	RBNode<T, D>* greatGrandParent = nullptr;
	RBNode<T, D>* sibling = this->GetSibling().get();
	// Traverse the tree
	while(!tree->End() && (*tree)->key != key)
	{
		
	}
	// Empty spot was found or key was found
	if((*tree)->key != key)
	{
		RBNode<T,D> * node = new RBNode<T, D>(key, data, parent, Color::RED);
		*tree = RedBlackTreeTopDown<T, D>(node);
	}
	return tree;
}

template <class T, class D>
void RedBlackTreeTopDown<T, D>::Delete(const T& key)
{
	
}

template <class T, class D>
void RedBlackTreeTopDown<T, D>::Rotate(bool left)
{
	RedBlackTreeTopDown<T, D> tree;
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
bool RedBlackTreeTopDown<T, D>::IsLeftChild()
{
	if (this->get()->parent == nullptr)
	{
		return false;
	}
	if (this->parent->get()->left == this)
		return true;
	return false;
}

template <class T, class D>
bool RedBlackTreeTopDown<T, D>::IsRoot()
{
	return (*this)->parent == nullptr;
}

template <class T, class D>
bool RedBlackTreeTopDown<T, D>::End()
{
	return this->get() == nullptr;
}

template <class T, class D>
int RedBlackTreeTopDown<T, D>::Depth()
{
	if (End())
	{
		return 0;
	}
	return max((*this)->left.Depth(), (*this)->right.Depth()) + 1;
}

template <class T, class D>
int RedBlackTreeTopDown<T, D>::NumberOfNodes()
{
	if (End())
	{
		return 0;
	}
	return (this->get()->left.NumberOfNodes() + this->get()->right.NumberOfNodes() + 1);
}

template <class T, class D>
RBNode<T, D>* RedBlackTreeTopDown<T, D>::GetParent()
{
	if (this->End())
	{
		return new RBNode<T, D>();
	}
	return (*this)->parent;
}

template <class T, class D>
RedBlackTreeTopDown<T, D>& RedBlackTreeTopDown<T, D>::GetSibling()
{
	if ((*this)->parent == nullptr)
	{
		throw "This node has no parent";
	}
	RBNode<T, D>* parent = (*this)->parent;
	if (this->get() == parent->left.get())
	{
		return parent->right;
	}
	return parent->left;
}

template <class T, class D>
RedBlackTreeTopDown<T, D>& RedBlackTreeTopDown<T, D>::GetUnderPredecessor()
{
	RedBlackTreeTopDown<T, D> under = this->left;
	while (!under.End() && !under.get()->right.End())
	{
		under = under.get()->right;
	}
	return under;
}

template <class T, class D>
void RedBlackTreeTopDown<T, D>::Write(ostream& os)
{
	if (!this->End())
	{
		this->get()->left.Write(os);
		os << this->get()->key;
		os << "(" << colorstring[static_cast<int>((*this)->color)] << ") ";
		os << (this->IsRoot() ? "root" : "") << endl;
		this->get()->right.Write(os);
	}
}

template <class T, class D>
RedBlackTreeTopDown<T, D>* RedBlackTreeTopDown<T, D>::Search(const T& search, RBNode<T, D>*& parent)
{
	if (this->End() || this->get()->key == search)
	{
		return this;
	}

	RedBlackTreeTopDown<T, D>* tree = this;
	while (!tree->End() && tree->get()->key != search)
	{
		tree->get()->parent = parent;
		parent = tree->get();
		if (tree->get()->key > search)
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
class RBNode
{
	friend class RedBlackTreeTopDown<T, D>;
public:
	RBNode() : color(Color::RED), parent(nullptr)
	{
	}

	RBNode(const T& k, const D& d) : RBNode(k, d, nullptr, Color::RED)
	{
	}

	RBNode(const T& k, const D& d, RBNode<T, D>* p, Color c) : key(k), data(d), color(c), parent(p)
	{
	}

	T key;
	D data;
	Color color;
	RBNode<T, D>* parent;
	RedBlackTreeTopDown<T, D> left;
	RedBlackTreeTopDown<T, D> right;

	RBNode<T, D>* GetSibling();
};

template <class T, class D>
RBNode<T, D>* RBNode<T, D>::GetSibling()
{
	RBNode<T, D>* parent = this->parent;
	if (parent->left.get() == this)
	{
		return parent->right.get();
	}
	return parent->left.get();
}
