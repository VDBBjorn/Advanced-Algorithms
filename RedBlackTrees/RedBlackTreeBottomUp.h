#pragma once
#include <string>
#include <cassert>
#include <memory>

using namespace std;

template <class T, class D>
class RedBlackTreeBottomUp;
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

string colorstring[] = {"B", "R", "BB"};

template <class T, class D>
class RedBlackTreeBottomUp : public RBNodePointer<T, D>
{
	using RBNodePointer<T, D>::RBNodePointer;
	friend class RBNode<T, D>;
public:
	RedBlackTreeBottomUp()
	{
	}

	explicit RedBlackTreeBottomUp(RBNode<T, D>*);
	RedBlackTreeBottomUp<T, D>* Search(const T& search);
	RedBlackTreeBottomUp<T, D>* Add(const T&, const D&);
	void Delete(const T&);

	void Rotate(bool);

	bool IsLeftChild();
	bool IsRoot();
	bool End();
	int Depth();
	int NumberOfNodes();
	RBNode<T, D>* GetParent();
	RedBlackTreeBottomUp<T, D> GetSibling();
	RedBlackTreeBottomUp<T, D> GetUnderPredecessor();

	void Write(ostream& os);

	friend ostream& operator<<(ostream& os, RedBlackTreeBottomUp<T, D>& t)
	{
		t.Write(os);
		return os;
	}

protected:
	RedBlackTreeBottomUp<T, D>* Search(const T&, RBNode<T, D>*&);
};

template <class T, class D>
RedBlackTreeBottomUp<T, D>::RedBlackTreeBottomUp(RBNode<T, D>* node)
{
	this->reset(std::move(node));
}

template <class T, class D>
RedBlackTreeBottomUp<T, D>* RedBlackTreeBottomUp<T, D>::Search(const T& search)
{
	if (this->End())
	{
		return this;
	}
	RBNode<T, D>* parent = this->get()->parent;
	return Search(search, parent);
}

template <class T, class D>
RedBlackTreeBottomUp<T, D>* RedBlackTreeBottomUp<T, D>::Add(const T& key, const D& data)
{
	RBNode<T, D>* parent = nullptr;
	RedBlackTreeBottomUp<T, D>* tree = Search(key, parent);
	RBNode<T, D>* node = nullptr;
	if (tree->End())
	{
		node = new RBNode<T, D>(key, data, parent, Color::RED);
		*tree = RedBlackTreeBottomUp<T, D>(node);
		bool doubleRed = (parent != nullptr && parent->color == Color::RED);
		while (doubleRed && !tree->End())
		{
			TurnBlack(node);
			parent = node->parent;
			doubleRed = (node->color == Color::RED && parent != nullptr && parent->color == Color::RED);
		}
	}
	// Root should be black
	if (node != nullptr && node->parent == nullptr)
	{
		node->color = Color::BLACK;
	}
	cout << *(this) << endl;
	return tree;
}

template <class T, class D>
void TurnBlack(RBNode<T, D>*& child)
{
	RBNode<T, D>* parent = child->parent;
	RBNode<T, D>* grandParent = parent->parent;
	// 1) De broer b van p is rood
	RBNode<T, D>* parentSibling = parent->GetSibling();
	if (parent->color == Color::RED && parentSibling->color == Color::RED)
	{
		parent->color = Color::BLACK;
		grandParent->color = Color::RED;
		parentSibling->color = Color::BLACK;
	}
	else
	{ // 2) De broer b van p is zwart.
		bool parentLeft = (parent == grandParent->left.get());
		bool childLeft = (child == parent->left.get());
		// Kind en ouder niet op 1 lijn 
		if (parentLeft != childLeft)
		{
			if (parentLeft)
			{
				grandParent->left.Rotate(true);
			}
			else
			{
				grandParent->right.Rotate(false);
			}
		}
		// Kind en ouder liggen op 1 lijn
		grandParent->color = Color::RED;
		parent->color = Color::BLACK;
		//TODO: grandParent->Rotate(!parentLeft);
	}
	child = grandParent;
}

template <class T, class D>
void RedBlackTreeBottomUp<T, D>::Delete(const T& key)
{
	RBNode<T, D>* parent = nullptr;
	RedBlackTreeBottomUp<T, D>* tree = Search(key, parent);
	if (tree->End())
	{
		return;
	}
	// We zoeken de ondervoorganger van deze knoop.
	RedBlackTreeBottomUp<T, D> underPredecessor = tree->GetUnderPredecessor();
	RedBlackTreeBottomUp<T, D>* toDelete = tree;
	// Als er 1 is dan 'wisselen' we deze en deleten we de knoop van de ONDERVOORGANGER. We zitten dan onderaan.
	if (!underPredecessor.End())
	{
		tree->get()->key = underPredecessor->key;
		tree->get()->data = underPredecessor->data;
		toDelete = underPredecessor;
	}
	// Is er geen ondervoorganger, dan zitten we al helemaal onderaan en kunnen we deze dus verwijderen.
	// toDelete is de fysisch te verwijderen boom.
	// Als die rood is, kunnen we die zonder meer verwijderen (kan geen kinderen meer hebben)
	if (toDelete->color == Color::RED)
	{
		delete toDelete;
	}
	else
	{
		// toDelete is zwart
		// 2 deelgevallen: 1 (links) rood kind of geen kinderen
		// Deelgeval 1: een links rood kind.
		RedBlackTreeBottomUp<T, D> leftChild = toDelete->get()->left;
		if (leftChild != NULL && leftChild->color == Color::RED)
		{
			//Schuif het kind omhoog
			toDelete->get()->key = leftChild->key;
			toDelete->get()->data = leftChild->data;
			delete leftChild->get();
		}
		// Deelgeval 2: de fysieke knoop is zwart zonder kinderen
		else
		{
			// Eerst maken we de toDelete een nulknoop die dubbelzwart is.
			toDelete = new RedBlackTreeBottomUp<T, D>();
			(*toDelete)->color = Color::DOUBLEBLACK;

			// Zolang de knoop dubbelzwart is, en een ouder heeft, proberen we de situatie te herstellen.
			while ((*toDelete)->color == Color::DOUBLEBLACK && (*toDelete)->parent != nullptr)
			{
				RedBlackTreeBottomUp<T, D> sibling = toDelete->GetSibling();
				// Als de broer rood is, roteer de ouder naar links
				if (toDelete->get()->parent->color == Color::RED)
				{
					toDelete->get()->parent->Rotate(true);
					toDelete->get()->parent->color = Color::RED;
					sibling->get()->color = Color::BLACK;
				}
				// Als de broer zwart is, onderscheiden we 3 gevallen
				else
				{
					// Geval 1a: De broer zijn beide kinderen zijn zwart.

					// Geval 1b: De broer heeft 1 rood rechter kind. (als c links hangt)

					// Geval 1c: De broer heeft 1 rood linker kind.

				}
			}
		}
	}
}

template <class T, class D>
void RedBlackTreeBottomUp<T, D>::Rotate(bool left)
{
	RedBlackTreeBottomUp<T, D> tree;
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
bool RedBlackTreeBottomUp<T, D>::IsLeftChild()
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
bool RedBlackTreeBottomUp<T, D>::IsRoot()
{
	return (*this)->parent == nullptr;
}

template <class T, class D>
bool RedBlackTreeBottomUp<T, D>::End()
{
	return this->get() == nullptr;
}

template <class T, class D>
int RedBlackTreeBottomUp<T, D>::Depth()
{
	if (End())
	{
		return 0;
	}
	return max((*this)->left.Depth(), (*this)->right.Depth()) + 1;
}

template <class T, class D>
int RedBlackTreeBottomUp<T, D>::NumberOfNodes()
{
	if (End())
	{
		return 0;
	}
	return (this->get()->left.NumberOfNodes() + this->get()->right.NumberOfNodes() + 1);
}

template <class T, class D>
RBNode<T, D>* RedBlackTreeBottomUp<T, D>::GetParent()
{
	if (this->End())
	{
		return new RBNode<T, D>();
	}
	return (*this)->parent;
}

template <class T, class D>
RedBlackTreeBottomUp<T, D> RedBlackTreeBottomUp<T, D>::GetSibling()
{
	if ((*this)->parent->End())
	{
		throw "This node has no parent";
	}
	RBNode<T, D>* parent = (*this)->parent;
	if (this->get() == parent->left->get())
	{
		return parent->right;
	}
	return parent->left;
}

template <class T, class D>
RedBlackTreeBottomUp<T, D> RedBlackTreeBottomUp<T, D>::GetUnderPredecessor()
{
	RedBlackTreeBottomUp<T, D> under = this->left;
	while (!under.End() && !under.get()->right.End())
	{
		under = under.get()->right;
	}
	return under;
}

template <class T, class D>
void RedBlackTreeBottomUp<T, D>::Write(ostream& os)
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
RedBlackTreeBottomUp<T, D>* RedBlackTreeBottomUp<T, D>::Search(const T& search, RBNode<T, D>*& parent)
{
	if (this->End() || this->get()->key == search)
	{
		return this;
	}

	RedBlackTreeBottomUp<T, D>* tree = this;
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
	friend class RedBlackTreeBottomUp<T, D>;
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
	RedBlackTreeBottomUp<T, D> left;
	RedBlackTreeBottomUp<T, D> right;

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
