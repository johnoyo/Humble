#pragma once
#include <iostream>

namespace HBL {

	// Define the character size
	#define CHAR_SIZE 128

	class Trie
	{
	public:
		bool isLeaf;
		Trie* character[CHAR_SIZE];
		int index = 0;

		Trie()
		{
			this->isLeaf = false;

			for (int i = 0; i < CHAR_SIZE; i++) {
				this->character[i] = nullptr;
			}
		}

		void insert(std::string, int);
		bool deletion(Trie*&, std::string);
		int search(std::string);
		bool haveChildren(Trie const*);
	};

}