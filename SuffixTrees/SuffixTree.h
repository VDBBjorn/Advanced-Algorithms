#pragma once
#include <string>
#include "SuffixNode.h"
#include "ActiveNode.h"

using namespace std;

class SuffixTree
{
public:
	SuffixTree(char end = -1);
	~SuffixTree();
	void Add(const string c);
private:
	void StartPhase(int);

	string text;
	char endChar;

	SuffixNode* root;
	ActiveNode* active;	
	/// <summary>
	/// The global end node
	/// </summary>
	EndNode* endNode;
	/// <summary>
	/// The remaining suffix count
	/// </summary>
	int remainingSuffixCount;

	SuffixNode* SelectNode(int index);
	SuffixNode* SelectNode();
	char& NextChar(int i);
};

inline SuffixTree::SuffixTree(char end) : endChar(end), root(nullptr), active(nullptr), endNode(nullptr), remainingSuffixCount(0)
{
}

inline SuffixTree::~SuffixTree()
{
}

inline void SuffixTree::Add(const string c)
{
	text = c;
	root = new SuffixNode(1, new EndNode(0));
	active = new ActiveNode(root);
	endNode = new EndNode(-1);
	for (int i = 0; i < c.length(); i++)
	{
		StartPhase(i);
	}
}

inline int Diff(SuffixNode* node) {
	return node->endNode->end - node->start;
}

inline SuffixNode* SuffixTree::SelectNode(int index) {
	return active->activeNode->children[text[index]];
}

inline SuffixNode* SuffixTree::SelectNode() {
	return active->activeNode->children[text[active->activeEdge]];
}


inline char& SuffixTree::NextChar(int i)
{
	SuffixNode* node = SelectNode();
	if(Diff(node) >= active->activeLength)
	{
		return text[active->activeNode->children[text[active->activeEdge]]->start + active->activeLength];
	}
	if(Diff(node)+1 == active->activeLength)
	{
		if(node->children[text[i]] != nullptr)
		{
			return text[i];
		}
	}
	else
	{
		active->activeNode = node;
		active->activeLength = active->activeLength - Diff(node) - 1;
		active->activeEdge = active->activeEdge + Diff(node) + 1;
		return NextChar(i);
	}
	throw exception("End of path reached");
}

inline void SuffixTree::StartPhase(int i)
{
	SuffixNode* lastCreatedInternalNode = nullptr;
	endNode->end++;
	remainingSuffixCount++;

	while(remainingSuffixCount>0)
	{
		// if activeLength = 0, look for current char from root
		if((*active).activeLength == 0)
		{
			// if there is a node at index i
			// increase active length and break out of while
			if(SelectNode(i) != nullptr)
			{
				(*active).activeEdge = SelectNode(i)->start;
				(*active).activeLength++;
				break;
			}
			// create a new leaf node with current character from leaf
			else
			{
				root->children[text[i]] = new SuffixNode(i, endNode);
				remainingSuffixCount--;
			}
		}
		// check if next char is same as current
		else
		{
			// next char is same, so point to active node
			if(text[i] == NextChar(i))
			{
				if (lastCreatedInternalNode != nullptr) {
					lastCreatedInternalNode->link = SelectNode();
				}
			}
			// next char is not same, so create new internal node
			else
			{
				SuffixNode* node = SelectNode();
				int oldStart = node->start;
				node->start = node->start + active->activeLength;
				SuffixNode* newInternalNode = new SuffixNode(oldStart, new EndNode(oldStart + active->activeLength - 1));
				SuffixNode* newLeafNode = new SuffixNode(i, endNode);

				//set internal nodes child as old node and new leaf node.
				newInternalNode->children[text[newInternalNode->start + active->activeLength]] = node;
				newInternalNode->children[text[i]] = newLeafNode;
				newInternalNode->index = -1;
				active->activeNode->children[text[newInternalNode->start]] = newInternalNode;

				//if another internal node was created in last extension of this phase then suffix link of that
				//node will be this node.
				if (lastCreatedInternalNode != nullptr) {
					lastCreatedInternalNode->link = newInternalNode;
				}

				//set this guy as lastCreatedInternalNode and if new internalNode is created in next extension of this phase
				//then point suffix of this node to that node. Meanwhile set suffix of this node to root.
				lastCreatedInternalNode = newInternalNode;
				newInternalNode->link = root;

				//if active node is not root then follow suffix link
				if (active->activeNode != root) {
					active->activeNode = active->activeNode->link;
				}
				//if active node is root then increase active index by one and decrease active length by 1
				else {
					active->activeEdge = active->activeEdge + 1;
					active->activeLength--;
				}
				remainingSuffixCount--;
			}
		}
	}
}
