#pragma once
#include <queue>
#include <memory>

using namespace std;

template <class T, class D>
class SearchTree;
template <class T, class D>
class SearchNode;

/// Name the unique pointer to a node
template <class T, class D>
using SearchNodePointer = unique_ptr<SearchNode<T, D>>;

template <class T, class D>
class SearchTree : public SearchNodePointer<T, D>
{
	using SearchNodePointer<T, D>::SearchNodePointer;
	friend class SearchNode<T, D>;
public:
	SearchTree()
	{
	}

	explicit SearchTree(SearchNode<T, D>* node);
	SearchTree(SearchTree<T, D>&& b) noexcept;
	SearchTree<T, D>& operator=(SearchTree<T, D>&& b) noexcept;

	SearchTree(const SearchTree<T, D>&) = delete;
	SearchTree& operator=(const SearchTree<T, D>&) = delete;

	virtual SearchTree<T, D>* Search(const T& search);
	virtual SearchTree<T, D>* Add(const T& key, const D& data);
	virtual void Delete(const T& key);
	virtual void Write(ostream& os);
	void Rotate(bool left);

	bool End();
	int Depth();
	int NumberOfNodes();
	SearchTree<T, D>* GetSibling();

	friend ostream& operator<<(ostream& os, SearchTree<T, D>& t)
	{
		t.Write(os);
		return os;
	}

protected:
	SearchTree<T, D>* Search(const T&, SearchNode<T, D>*&);
};

template <class T, class D>
SearchTree<T, D>::SearchTree(SearchNode<T, D>* node)
{
	this->reset(std::move(node));
}

template <class T, class D>
SearchTree<T, D>::SearchTree(SearchTree<T, D>&& b) noexcept: unique_ptr<Node<T, D>>(move(b))
{
}

template <class T, class D>
SearchTree<T, D>& SearchTree<T, D>::operator=(SearchTree<T, D>&& b) noexcept
{
	if (this != &b)
	{
		this->reset(b.release());
	}
	return *this;
}

template <class T, class D>
bool SearchTree<T, D>::End()
{
	return this->get() == nullptr;
}

template <class T, class D>
int SearchTree<T, D>::Depth()
{
	if (End())
	{
		return 0;
	}
	return max((*this)->left.Depth(), (*this)->right.Depth()) + 1;
}

template <class T, class D>
int SearchTree<T, D>::NumberOfNodes()
{
	if (End())
	{
		return 0;
	}
	return (this->get()->left.NumberOfNodes() + this->get()->right.NumberOfNodes() + 1);
}

template <class T, class D>
SearchTree<T, D>* SearchTree<T, D>::GetSibling()
{
	if (this->parent->End())
	{
		return new SearchTree();
	}
	SearchTree<T, D>* parent = this->parent;
	if (this == parent->get()->left)
	{
		return parent->get()->right;
	}
	return parent->get()->left;
}

template <class T, class D>
SearchTree<T, D>* SearchTree<T, D>::Search(const T& search, SearchNode<T, D>*& parent)
{
	if (this->End() || this->get()->key == search)
	{
		return this;
	}

	SearchTree<T, D>* tree = this;
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
SearchTree<T, D>* SearchTree<T, D>::Search(const T& search)
{
	if (this->End())
	{
		return this;
	}
	SearchNode<T, D>* parent = static_cast<SearchNode<T, D>*>(this->get()->parent);
	return Search(search, parent);
}

template <class T, class D>
SearchTree<T, D>* SearchTree<T, D>::Add(const T& key, const D& data)
{
	SearchTree<T, D>* tree = Search(key);
	if (tree->End())
	{
		*tree = static_cast<SearchTree<T, D>>(static_cast<unique_ptr<SearchNode<T, D>>>(new SearchNode<T, D>(key, data)));
	}
	return tree;
}

template <class T, class D>
void SearchTree<T, D>::Delete(const T& key)
{
	throw "not implemented";
}

/// <summary>
/// Rotates the tree
/// </summary>
/// <param name="left">if set to <c>true</c> the tree gets rotated left. If set to <c>false</c> the tree gets rotated right.</param>
template <class T, class D>
void SearchTree<T, D>::Rotate(bool left)
{
	SearchTree<T, D> tree;
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
void SearchTree<T, D>::Write(ostream& os)
{
	if (!this->End())
	{
		if(!this->get()->left.End())
			this->get()->left.Write(os);
		os << this->get()->key << " ";
		if (!this->get()->right.End())
			this->get()->right.Write(os);
	}
}

template <class T, class D>
class SearchNode
{
	friend class SearchTree<T, D>;
public:
	SearchNode()
	{
	}

	SearchNode(const T& k, const D& d);
	T key;
	D data;
	SearchNode<T, D>* parent;
	SearchTree<T, D> left;
	SearchTree<T, D> right;
};


template <class T, class D>
SearchNode<T, D>::SearchNode(const T& k, const D& d): key(k), data(d)
{
}
