#pragma once

template <class T, class D, unsigned int m>
class BTree;
template <class T, class D, unsigned int m>
class BNode;

template <class T, class D, unsigned int m>
class BTree
{
	typedef BNode<T, D, m> Node;

};

template <class T, class D, unsigned int m>
class BNode
{
	friend class BTree<T, D, m>;

};
