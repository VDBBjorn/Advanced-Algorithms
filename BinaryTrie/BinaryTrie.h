#pragma once
#include <memory>
#include <vector>
#include "BitPattern.h"
typedef unsigned int uint;
using namespace std;

class Node
{
	friend class BTrie;
public:
	Node() {
	}
	virtual ~Node() {};
	virtual bool isLeaf();
	virtual void print(ostream & os)const;
};

class InternalNode: public Node
{
public:
	InternalNode();
	vector<BTrie*> children;
};

class BTrie: unique_ptr<Node> {
public:
	BTrie() {
	}

	BTrie(Node*);
	void Add(BitPattern& bit);
	BTrie* Search(BitPattern& bit, uint& bitplaats);
};

inline bool Node::isLeaf()
{
	return true;
}

inline void Node::print(ostream& os) const
{
}

inline InternalNode::InternalNode()
{
	children.resize(2);
	for(int i=0; i<children.size(); i++)
	{
		children[i] = new BTrie();
	}
}

inline BTrie::BTrie(Node* node)
{
	this->reset(move(node));
}

inline void BTrie::Add(BitPattern& bit)
{

}

inline BTrie* BTrie::Search(BitPattern& bit, uint& bitPosition)
{
	BTrie* iterator = this;
	while(!(*iterator)->isLeaf())
	{
		InternalNode* internalNode = static_cast<InternalNode*>(iterator->get());
		iterator = internalNode->children[bit.en(BitPattern::eenbit(bitPosition))];
		bitPosition++;
	}
	return iterator;
}
