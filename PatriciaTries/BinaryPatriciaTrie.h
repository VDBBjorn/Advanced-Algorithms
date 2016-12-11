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
	if ((*this)->index <= index)
	{
		//huidige index kleiner dan k dat een kind is
		//terug verbinding of knoop naar jezelf, stoppen met zoeken
		return this;
	}
	if (key.bit((*this)->index)) { 
		return (*this)->right->Search(key, (*this)->index);
	}
	return (*this)->left->Search(key, (*this)->index);
}

inline BinaryPatriciaTrie* BinaryPatriciaTrie::Add(const Key& key)
{
	return Add(key, -1);
}

inline BinaryPatriciaTrie* BinaryPatriciaTrie::Add(const Key& key, int index)
{
	//1. grotere index gevonden dan verschil index
	// -> nieuwe knoop tussen voegen
	// -> sleutel in knoop opslaan
	// -> gepaste wijzer naar knoop wijzen
	/*if()
	{
		
	}*/
	//2. geen knoop met hogere index, je zit eigenlijk nog steeds parent zijn links of rechts aan te passen
	//eidigen in blad	
}
