#pragma once
#include <memory>
#include "Key.h"

class PatriciaNode;
class BinaryPatriciaNode;

using namespace std;

class BinaryPatriciaTrie: unique_ptr<BinaryPatriciaNode>
{
	using unique_ptr<BinaryPatriciaNode>::unique_ptr;
	BinaryPatriciaTrie* Search(const Key& key);
	BinaryPatriciaTrie* Add(const Key&);
private:
	BinaryPatriciaTrie* Search(const Key& key, int index);
	BinaryPatriciaTrie* Add(const Key&, int index);
};

class PatriciaNode
{
public:
	PatriciaNode(const Key& key, int index) : key(key), index(index)
	{
	}

	Key key;
	int index;
};

class BinaryPatriciaNode : public PatriciaNode
{
public:
	BinaryPatriciaNode(const Key& key, int index)
		: PatriciaNode(key, index), left(nullptr), right(nullptr)
	{
	}
	BinaryPatriciaTrie* left;
	BinaryPatriciaTrie* right;
};

inline BinaryPatriciaTrie* BinaryPatriciaTrie::Search(const Key& key)
{
	return Search(key, -1);
}

inline BinaryPatriciaTrie* BinaryPatriciaTrie::Search(const Key& key, int index)
{
	if ((*this)->index <= index) //terugverbinding of verwijst naar jezelf = stoppen
	{
		return this;
	}
	if (key.bit((*this)->index)) { 
		return (*this)->right->Search(key, (*this)->index);
	}
	return (*this)->left->Search(key, (*this)->index);
}

inline BinaryPatriciaTrie* BinaryPatriciaTrie::Add(const Key& key)
{
	BinaryPatriciaTrie * search = Search(key, -1);
	if((*search)->key == key) //bestaat al!
	{
		return search;
	}
	int difference = (*search)->key.difference(key);
	cout << "Difference between current key and key to add: " << difference << endl;


}

inline BinaryPatriciaTrie* BinaryPatriciaTrie::Add(const Key& key, int difference)
{

	if (key.bit((*this)->index)) {
		(*this)->right = (*this)->right->Add(key, (*this)->index);
	}
	else {
		(*this)->left = (*this)->left->Add(key, (*this)->index);
	}
	return this;
}
