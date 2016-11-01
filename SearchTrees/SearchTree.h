#pragma once
#include <queue>
#include <memory>

using namespace std;

/// Definition of classes
template <class T, class D>
class SearchTree;
template <class T, class D>
class SearchNode;

/// Name the unique pointer to a node
template <class T, class D>
using NodePointer = unique_ptr<SearchNode<T, D>>;

template <class T, class D>
class SearchTree : public NodePointer<T, D>
{
private:
	using NodePointer<T, D>::NodePointer;
	friend class SearchNode<T, D>;
public:
	SearchTree()
	{
	}

	~SearchTree()
	{
	}

	explicit SearchTree(SearchNode<T, D>* node);
	SearchTree<T, D>& operator=(SearchTree<T, D>&& b) noexcept;

	SearchTree(const SearchTree<T, D>&) = delete;
	SearchTree& operator=(const SearchTree<T, D>&) = delete;

	SearchTree<T, D>* Search(const T& search);
	SearchTree<T, D>* Add(const T& key, const D& data);
	void Delete(const T& key);
	void Rotate(bool left);
	void Write(ostream& os);
	bool End();
	int Depth();
	int NumberOfNodes();
	SearchNode<T, D>* GetParent();
	SearchTree<T, D>& GetSibling();

	friend ostream& operator<<(ostream& os, SearchTree<T, D>& t)
	{
		t.Write(os);
		return os;
	}

protected:
	SearchTree<T, D>* Search(const T&, SearchNode<T, D>*&);
};



template <class T, class D>
class SearchNode
{
public:
	SearchNode() : parent(nullptr)
	{

	}

	SearchNode(const T& k, const D& d) : SearchNode(k, d, nullptr)
	{
	}

	SearchNode(const T& k, const D& d, SearchNode<T, D>* p) : key(k), data(d), parent(p)
	{
	}

	T key;
	D data;
	SearchNode<T, D>* parent;
	SearchTree<T, D> left;
	SearchTree<T, D> right;
};

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
SearchTree<T, D>::SearchTree(SearchNode<T, D>* node)
{
	this->reset(std::move(node));
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
SearchTree<T, D>* SearchTree<T, D>::Search(const T& search)
{
	if (this->End())
	{
		return this;
	}
	SearchNode<T, D>* parent = this->get()->parent;
	return Search(search, parent);
}

template <class T, class D>
SearchTree<T, D>* SearchTree<T, D>::Add(const T& key, const D& data)
{
	SearchNode<T, D>* parent = this->GetParent();
	SearchTree<T, D>* tree = Search(key, parent);
	if (tree->End())
	{
		*tree = static_cast<SearchTree<T, D>>(static_cast<unique_ptr<SearchNode<T, D>>>(new SearchNode<T, D>(key, data)));
		(*tree)->parent = parent;
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
		this->get()->left.Write(os);
		os << this->get()->key << " (" << this->get()->parent->key << ") ";
		this->get()->right.Write(os);
	}
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
SearchNode<T, D>* SearchTree<T, D>::GetParent()
{
	if(this->End())
	{
		return new SearchNode<T, D>();
	}
	return (*this)->parent;
}

template <class T, class D>
SearchTree<T, D>& SearchTree<T, D>::GetSibling()
{
	if ((*this)->parent->End())
	{
		throw "This node has no parent";
	}
	SearchNode<T, D>* parent = (*this)->parent;
	if (this->get() == parent->left->get())
	{
		return parent->right;
	}
	return parent->left;
}
