#include "Trie.h"

namespace HBL {

	// Iterative function to insert a key into a Trie
	void Trie::insert(std::string key, int value)
	{
		// start from the root node
		Trie* curr = this;
		for (int i = 0; i < key.length(); i++)
		{
			// create a new node if the path doesn't exist
			if (curr->character[key[i]] == nullptr) {
				curr->character[key[i]] = new Trie();
			}

			// go to the next node
			curr = curr->character[key[i]];
		}

		// mark the current node as a leaf
		curr->isLeaf = true;
		curr->index = value;
	}

	// Iterative function to search a key in a Trie. It returns true
	// if the key is found in the Trie; otherwise, it returns false
	int Trie::search(std::string key)
	{
		// return false if Trie is empty
		if (this == nullptr) {
			return -1;
		}

		Trie* curr = this;
		for (int i = 0; i < key.length(); i++)
		{
			// go to the next node
			curr = curr->character[key[i]];

			// if the string is invalid (reached end of a path in the Trie)
			if (curr == nullptr) {
				return -1;
			}
		}

		// return true if the current node is a leaf and the
		// end of the string is reached
		if (curr->isLeaf)
			return curr->index;
		else
			return -1;
	}

	// Returns true if a given node has any children
	bool Trie::haveChildren(Trie const* curr)
	{
		for (int i = 0; i < CHAR_SIZE; i++)
		{
			if (curr->character[i]) {
				return true;	// child found
			}
		}

		return false;
	}

	// Recursive function to delete a key in the Trie
	bool Trie::deletion(Trie*& curr, std::string key)
	{
		// return if Trie is empty
		if (curr == nullptr) {
			return false;
		}

		// if the end of the key is not reached
		if (key.length())
		{
			// recur for the node corresponding to the next character in the key
			// and if it returns true, delete the current node (if it is non-leaf)

			if (curr != nullptr &&
				curr->character[key[0]] != nullptr &&
				deletion(curr->character[key[0]], key.substr(1)) &&
				curr->isLeaf == false)
			{
				if (!haveChildren(curr))
				{
					delete curr;
					curr = nullptr;
					return true;
				}
				else {
					return false;
				}
			}
		}

		// if the end of the key is reached
		if (key.length() == 0 && curr->isLeaf)
		{
			// if the current node is a leaf node and doesn't have any children
			if (!haveChildren(curr))
			{
				// delete the current node
				delete curr;
				curr = nullptr;

				// delete the non-leaf parent nodes
				return true;
			}

			// if the current node is a leaf node and has children
			else {
				// mark the current node as a non-leaf node (DON'T DELETE IT)
				curr->isLeaf = false;

				// don't delete its parent nodes
				return false;
			}
		}

		return false;
	}

}

