#pragma once
#include "EndNode.h"
#include <vector>

class SuffixNode
{
	friend class SuffixTree;
public:
	SuffixNode();
	SuffixNode(int start, EndNode* end);
	virtual ~SuffixNode()
	{
	}

	virtual bool IsLeaf();


	int start;
	int end;
	int index;
	EndNode* endNode;
	SuffixNode* link;
	std::vector<SuffixNode*> children;
};

inline SuffixNode::SuffixNode(): start(0), end(0), index(-1), endNode(nullptr), link(nullptr)
{
	children = std::vector<SuffixNode*>();
}

inline SuffixNode::SuffixNode(int start, EndNode* end) : start(start), end(end->end), index(-1), endNode(end), link(nullptr)
{
	children = std::vector<SuffixNode*>();
}

inline bool SuffixNode::IsLeaf()
{
	return false;
}
