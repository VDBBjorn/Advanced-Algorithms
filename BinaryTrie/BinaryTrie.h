#pragma once
#include <memory>
#include <vector>
#include "BitPattern.h"
class Node;
typedef unsigned int uint;
using namespace std;

class BTrie : unique_ptr<Node>
{
	using unique_ptr<Node>::unique_ptr;
public:
	BTrie()
	{
	}

	BTrie& operator=(BTrie&& b) noexcept;

	BTrie(Node*);
	void Add(BitPattern& bit);
	BTrie* Search(BitPattern& bit, uint& bitplaats);
};

class Node
{
	friend class BTrie;
public:
	Node()
	{
	}

	virtual ~Node()
	{
	};
	virtual bool isLeaf();
	virtual void print(ostream& os) const;
};

class InternalNode: public Node
{
public:
	InternalNode();
	vector<BTrie*> children;
};

class LeafNode: public Node
{
public:
	LeafNode()
	{
	}

	explicit LeafNode(BitPattern& bit)
	{
		pattern = bit;
	}

	bool isLeaf() override;
	BitPattern pattern;
};

inline bool Node::isLeaf()
{
	return false;
}

inline void Node::print(ostream& os) const
{
}

inline InternalNode::InternalNode()
{
	children.resize(2);
	for (int i = 0; i < children.size(); i++)
	{
		children[i] = new BTrie();
	}
}

inline bool LeafNode::isLeaf()
{
	return true;
}

inline BTrie& BTrie::operator=(BTrie&& b) noexcept
{
	if (this != &b)
	{
		this->reset(b.release());
	}
	return *this;
}

inline BTrie::BTrie(Node* node)
{
	this->reset(move(node));
}

inline void BTrie::Add(BitPattern& bit)
{
	uint bitPosition;
	BTrie* trie = Search(bit, bitPosition);
	if(trie->get() == nullptr) // empty tree -> add new "leaf"
	{
		*trie = static_cast<BTrie>(make_unique<LeafNode>(bit));
	}
	if ((*trie)->isLeaf())
	{
		if (static_cast<LeafNode*>(trie->get())->pattern == bit) // don't add
		{
			return;
		}
		//blad, maar niet gelijk -> toevoegen van interne knopen tot verschillend bit of lengteverschil
		//TODO
	}
}

inline BTrie* BTrie::Search(BitPattern& bit, uint& bitPosition)
{
	BTrie* iterator = this;
	while (iterator->get() != nullptr && !(*iterator)->isLeaf())
	{
		InternalNode* internalNode = static_cast<InternalNode*>(iterator->get());
		iterator = internalNode->children[bit.en(BitPattern::eenbit(bitPosition))];
		bitPosition++;
	}
	return iterator;
}
