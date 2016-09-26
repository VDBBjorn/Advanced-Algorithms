#include <iostream>
#include <queue>
#include <memory>

using namespace std;

enum Color { RED, BLACK, DOUBLEBLACK };
/// Definition of classes
template<class T, class D>
class RedBlackTree;
template<class T, class D>
class RedBlackNode;
template<class T, class D>
class NullTree;
template<class T, class D>
class NullNode;

/// Name the unique pointer to a node
template<class T, class D>
using RedBlackNodePointer = std::unique_ptr<RedBlackNode<T, D> >;

/// <summary>
/// Red-black tree
/// </summary>
template<class T, class D>
class RedBlackTree : public RedBlackNodePointer<T, D> {
	using RedBlackNodePointer<T, D>::RedBlackNodePointer;
	friend class RedBlackNode<T, D>;
public:
};


/// <summary>
/// Red-black node
/// </summary>
template<class T, class D>
class RedBlackNode {
	friend class RedBlackTree<T, D>;
public:
	
};

/// <summary>
/// Representation of a null-node
/// </summary>
template<class T, class D>
class NullNode : public RedBlackNode<T, D> {
public:
	
};

/// <summary>
/// Representation of a null-tree
/// </summary>
template<class T, class D>
class NullTree : public RedBlackTree<T, D> {
public:
	
};
