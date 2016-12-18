#pragma once
#include <map>
#include "SuffixNode.h"

class SuffixChildNode : public std::map<char, SuffixNode*> {
public:
	SuffixChildNode() = default;
	~SuffixChildNode();

	void Set(char a, SuffixNode* p);
	SuffixNode* Get(char a) const;
};

inline SuffixChildNode::~SuffixChildNode()
{
	for (auto it = this->begin(); it != this->end(); it++) {
		delete it->second;
	}
}

inline void SuffixChildNode::Set(char a, SuffixNode* p)
{
	operator[](a) = p;
}

inline SuffixNode* SuffixChildNode::Get(char a) const
{
	const_iterator plaats = find(a);
	if (plaats == end())
		return nullptr;
	else
		return plaats->second;
}