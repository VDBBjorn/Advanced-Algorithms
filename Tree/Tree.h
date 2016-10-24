#pragma once
#include <queue>
#include <memory>

using namespace std;

/// Definition of classes
template <class T, class D>
class Tree;
template <class T, class D>
class Node;

/// Name the unique pointer to a node
template <class T, class D>
using NodePointer = unique_ptr<Node<T, D>>;

template <class T, class D>
class Tree : public NodePointer<T, D>
{
private:
	using NodePointer<T, D>::NodePointer;
	friend class Node<T, D>;
public:
	Tree()
	{
	}

	explicit Tree(Node<T, D>* node);
	Tree(Tree<T, D>&& b) noexcept;
	Tree<T, D>& operator=(Tree<T, D>&& b) noexcept;

	Tree(const Tree<T, D>&) = delete;
	Tree& operator=(const Tree<T, D>&) = delete;

	bool End();
	int Depth();
	int NumberOfNodes();

	virtual Tree<T, D>* Search(const T&) = 0;
	virtual Tree<T, D>* Add(const T&, const D&) = 0;
	virtual void Delete(const T&) = 0;
	virtual void Rotate(bool) = 0;

	virtual void Write(ostream& os);
	friend ostream& operator<<(ostream& os, Tree<T, D>& t)
	{
		t.Write(os);
		return os;
	}
};

template <class T, class D>
Tree<T, D>::Tree(Node<T, D>* node)
{
	this->reset(std::move(node));
}

template <class T, class D>
Tree<T, D>::Tree(Tree<T, D>&& b) noexcept: unique_ptr<Node<T, D>>(move(b))
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
	return this->get();
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
	return (this->get()->left->NumberOfNodes() + this->get()->right->NumberOfNodes() + 1);
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
class Node
{
	friend class Tree<T, D>;
public:

	Node(const T& k, const D& d) : key(k), data(d)
	{
	}

	T key;
	D data;
	Tree<T, D> left;
	Tree<T, D> right;
};
