#pragma once
#include "SuffixNode.h"

class ActiveNode
{
public:
	ActiveNode(SuffixNode*);

	SuffixNode* activeNode;
	int activeEdge;
	int activeLength;
};

inline ActiveNode::ActiveNode(SuffixNode* suffixnode): activeNode(suffixnode), activeLength(0), activeEdge(-1)
{
}
