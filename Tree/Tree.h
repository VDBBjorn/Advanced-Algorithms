#pragma once
#include <queue>
#include <memory>

using namespace std;

/// Definition of classes
template <class T, class D>
class Tree;
template <class T, class D>
class SearchNode;

/// Name the unique pointer to a node
template <class T, class D>
using NodePointer = unique_ptr<SearchNode<T, D>>;

template <class T, class D>
class Tree : public NodePointer<T, D>
{
protected:
	~Tree()
	{
		this->release();
	}

private:
	using NodePointer<T, D>::NodePointer;
	friend class SearchNode<T, D>;
public:
	Tree()
	{
	}

	explicit Tree(SearchNode<T, D>* node);
	Tree(Tree<T, D>&& b) noexcept;
	Tree<T, D>& operator=(Tree<T, D>&& b) noexcept;

	Tree(const Tree<T, D>&) = delete;
	Tree& operator=(const Tree<T, D>&) = delete;

	virtual Tree<T, D>* Search(const T&) = 0;
	virtual Tree<T, D>* Add(const T&, const D&) = 0;
	virtual void Delete(const T&) = 0;
	virtual void Rotate(bool) = 0;

	bool End();
	int Depth();
	int NumberOfNodes();
	Tree<T, D>& GetSibling();

	virtual void Write(ostream& os);

	friend ostream& operator<<(ostream& os, Tree<T, D>& t)
	{
		t.Write(os);
		return os;
	}
};

template <class T, class D>
Tree<T, D>::Tree(SearchNode<T, D>* node)
{
	this->reset(std::move(node));
}

template <class T, class D>
Tree<T, D>::Tree(Tree<T, D>&& b) noexcept: unique_ptr<SearchNode<T, D>>(move(b))
{
}

template <class T, class D>
Tree<T, D>& Tree<T, D>::operator=(Tree<T, D>&& b) noexcept
{
	if (this != &b)
	{
		this->reset(b.release());
	}
	return *this;
}

template <class T, class D>
bool Tree<T, D>::End()
{
	return this->get() == nullptr;
}

template <class T, class D>
int Tree<T, D>::Depth()
{
	if (End())
	{
		return 0;
	}
	return max((*this)->left.Depth(), (*this)->right.Depth()) + 1;
}

template <class T, class D>
int Tree<T, D>::NumberOfNodes()
{
	if (End())
	{
		return 0;
	}
	return (this->get()->left.NumberOfNodes() + this->get()->right.NumberOfNodes() + 1);
}

template <class T, class D>
Tree<T, D>& Tree<T, D>::GetSibling()
{
	if ((*this)->parent->End())
	{
		throw "This node has no parent";
	}
	Tree<T, D>* parent = (*this)->parent;
	if (this->get() == parent->left->get())
	{
		return parent->right;
	}
	return parent->left;
}


template <class T, class D>
void Tree<T, D>::Write(ostream& os)
{
	if (this->get() != nullptr)
	{
		this->get()->left.Write(os);
		os << this->get()->key << " ";
		this->get()->right.Write(os);
	}
}

template <class T, class D>
class SearchNode
{
	friend class Tree<T, D>;
public:

	SearchNode(const T& k, const D& d) : SearchNode(k, d, nullptr)
	{
	}

	SearchNode(const T& k, const D& d, SearchNode<T, D>* p) : key(k), data(d), parent(p)
	{
	}

	T key;
	D data;
	SearchNode<T, D>* parent;
	Tree<T, D> left;
	Tree<T, D> right;
};
