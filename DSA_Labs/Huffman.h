/*
File:			Huffman.h
Author(s):
	Base:		Justin Tackett
				jtackett@fullsail.com
	Student:

Created:		03.14.2021
Last Modified:	03.14.2021
Purpose:		A class to wrap up Huffman compression algorithm
Notes:			Property of Full Sail University
*/

// Header protection
#pragma once

/************/
/* Includes */
/************/
#include <queue>
#include "BitStream.h"


/***********/
/* Defines */
/***********/

/*
How to use:

	When working on a lab, turn that lab's #define from 0 to 1

		Example:	#define LAB_1	1

	When working on an individual unit test, turn that #define from 0 to 1

		Example:	#define DYNARRAY_DEFAULT_CTOR	1

NOTE: If the unit test is not on, that code will not be compiled!
*/

// Master toggle
#define LAB_8 1

// Individual unit test toggles
#define HUFFMAN_CTOR					1
#define HUFFMAN_GENERATE_FREQUENCY		1
#define HUFFMAN_GENERATE_LEAFLIST		1
#define HUFFMAN_GENERATE_TREE			1
#define HUFFMAN_CLEAR_TREE				1
#define HUFFMAN_DTOR					1
#define HUFFMAN_GENERATE_ENCODING		1
#define HUFFMAN_COMPRESS				1
#define HUFFMAN_DECOMPRESS				1

using namespace std;

#if LAB_8
// Wraps up Huffman compression algorithm
class Huffman {

	friend class DSA_TestSuite_Lab8; // Giving access to test code

	// A node for a a Huffman tree
	struct HuffNode {
		short value;		// The value to store (-1 if not a leaf node, 0-255 for the ascii value)
		unsigned int freq;	// The frequency of the node
		HuffNode* left;		// Left child
		HuffNode* right;	// Right child
		HuffNode* parent;	// Parent node

		// Constructor
		// 
		// In:	_value		The value of the node
		//		_freq		The frequency of the node
		//		_left		The node's left child (OPTIONAL)
		//		_right		The node's right child (OPTIONAL)
		//		_parent		The parent node (OPTIONAL)
		HuffNode(short _value, unsigned int _freq, HuffNode* _left = nullptr, HuffNode* _right = nullptr, HuffNode* _parent = nullptr)
			: value(_value), freq(_freq), left(_left), right(_right), parent(_parent) {
		}

		// Copy constructor
		//
		// In:	_hnode		The node to copy from
		HuffNode(const HuffNode& _hnode)
			: value(_hnode.value), freq(_hnode.freq), left(_hnode.left), right(_hnode.right), parent(_hnode.parent) {
		}
	};

	// A binary predicate used to compare HuffNodes
	//		This is used in the GenerateTree method
	struct HuffCompare {
		// Compare the frequency of two nodes
		// 
		// In:	_a		The first node
		//		_b		The second node
		//
		// Return: True, if _a's freq is greater than _b's freq
		bool operator() (const HuffNode* _a, const HuffNode* _b) {
			return(_a->freq > _b->freq);
		}
	};


	// Data members
	std::string mFileName;					// The name of the file to open for read/write
	unsigned int mFrequencyTable[257];		// The frequency of each character in the file (the index in the array is the char's ASCII value)
											// [256] of the array is the total count of characters in the file
	vector<HuffNode*> mLeafList;			// Contains all of the dynamically allocated leaf nodes
	HuffNode* mRoot;						// The root of the huffman tree (set in GenerateTree)
	vector<bool> mEncodingTable[256];		// Contains all of the bit-codes used to compress/decompress a file

	// Default constructor
	//
	// In:	_fileName		The name of the file to open
	//		_access			The way to open the file
	Huffman(const std::string& _fileName) {
		
		mFileName = _fileName;
		
		for (int i = 0; i < 257; i++) {

			mFrequencyTable[i] = 0;
		}

		mRoot = nullptr;
		
	}

	// Destructor
	//		Needs to clean up any left-over dynamic memory in tree
	~Huffman() {
		
		ClearTree();
	}


	// Generate the frequency table for the Huffman algorithm (WRITING ONLY)
	//
	// NOTE:	mFrequencyTable is an array of 257 unsigned ints representing the 
	//			count of each character in the file (the index in the array is the char's ASCII value)
	//				Example:  'A' is ASCII 65, so if A is the character being checked, [65] of your array would get incremented
	//			[256] of your array is the total count of characters in the file
	void GenerateFrequencyTable() {
		 
		char table;
		ifstream file(mFileName, ios::in | ios::binary);
			
		for (int i = 0; i < 257; i++) {

			mFrequencyTable[i] = 0;
		}
		
		while (file.get(table))
		{
			mFrequencyTable[(int)table]++;
			mFrequencyTable[256]++; 
		}
		
		file.close();
		
	}

	// Generate the leaf list for the Huffman algorithm (used in READ AND WRITE)
	//
	// Note:	Will cause leaks until ClearTree is implemented
	void GenerateLeafList() {
	
		for (int i = 0; i < 256; i++)
		{
			if (mFrequencyTable[i] > 0) {
				
				mLeafList.push_back(new HuffNode(i, mFrequencyTable[i]));
			}
				
		}

	}

	// Generate a Huffman tree
	void GenerateTree() {
		
		priority_queue<HuffNode*, vector<HuffNode*>, HuffCompare> tree;

		
		for (int i = 0; i < mLeafList.size(); i++)
		{
			tree.push(mLeafList[i]);
		}
		
		while (1 < tree.size()) {

			HuffNode* tL(tree.top());
			tree.pop();

			HuffNode* tR(tree.top());
			tree.pop();

			HuffNode* parent = new HuffNode(-1, tL->freq + tR->freq);

			parent->left = tL;
			parent->right = tR;

			tL->parent = parent;
			tR->parent = parent;

			tree.push(parent);
		}
		
		mRoot = tree.top();
	}

	// Generating the encoding table for the Huffman algorithm
	//
	//				Each vector will contain the bit-code corresponding to that index in the frequency table
	void GenerateEncodingTable() {
		
		for (int i = 0; i < mLeafList.size(); i++) {

			HuffNode* table = mLeafList[i];

			while (table != mRoot) {

				if (table->parent->left == table) {

					mEncodingTable[mLeafList[i]->value].push_back(0);
				}
				else if (table->parent->right == table) {

					mEncodingTable[mLeafList[i]->value].push_back(1);
				}
				table = table->parent;
			}

			reverse(mEncodingTable[mLeafList[i]->value].begin(), mEncodingTable[mLeafList[i]->value].end());
		}

		return;
	}

	// Clear the tree of all dynamic memory (by using the helper function)
	void ClearTree() {
		
		ClearTree(mRoot);
		mRoot = nullptr;
	}

	// Clear the tree of all dynamic memory (recursive helper function)
	// 
	// In:	_curr		The current node to clean up
	//
	// Note:	This will be a recursive function that does a post-order deletion
	void ClearTree(HuffNode* _curr) {
		
		if (!_curr) return;

		ClearTree(_curr->right);
		ClearTree(_curr->left);
		
		delete[] _curr;
	}

	// Write a Huffman compressed file to disk
	//
	// In:	_outputFile			Where to write the compressed data to
	//
	// Note: You will use most of your other functionality to complete this function
	void Compress(const char* _outputFile) {
		
		GenerateFrequencyTable();
		GenerateLeafList();
		GenerateTree();
		GenerateEncodingTable();
		
		BitOStream outfile(_outputFile, (char*)mFrequencyTable, sizeof(unsigned int) * 257);

		ifstream infile(mFileName, ios_base::binary);
		
		char table;

		while(infile.get(table))
		{
			outfile << mEncodingTable[(int)table];
		}

		infile.close();
		outfile.Close();
		
		ClearTree();

	}

	// Decompress a huffman-compressed file
	//
	// In:	_outputFile		Where to write the uncompressed data to
	//
	// Note: The mFileName will be the compressed file
	void Decompress(const char* _outputFile) {

		BitIStream infile(mFileName.c_str(), (char*)&mFrequencyTable, sizeof(unsigned int) * 257);

		GenerateLeafList();
		GenerateTree();

		ofstream outfile(_outputFile, ios::out | ios::binary);
		
		char table = NULL;
		bool travers;
		
		HuffNode* huff = mRoot;
		
		for (int i = 0; i < mFrequencyTable[256]; i++)
		{
			if (infile.eof()) break;

			for (int j = 0; j < 8; j++)
			{

				if (infile.eof()) break;

				if (isALeaf(huff))
				{
					outfile << (char)huff->value;
					huff = mRoot;
				}
				else {

					infile >> travers;

					if (travers)
					{
						huff = huff->right;
					}
					else
					{
						huff = huff->left;
					}
				}
			}
		}
		
		infile.Close();
		outfile.close();
		
		ClearTree();
	}

	bool isALeaf(HuffNode* _node) {
		if (_node->left || _node->right) return false;
		else return true;
	}

};

#endif	// End LAB_8