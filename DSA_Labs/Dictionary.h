/*
File:			DList.h
Author(s):
	Base:		Justin Tackett
				jtackett@fullsail.com
	Student:

Created:		1.17.2021
Last Modified:	2.26.2021
Purpose:		A hash-mapped data structure using key/value pairs and separate chaining
Notes:			Property of Full Sail University
*/

// Header protection
#pragma once

using namespace std;

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
#define LAB_5	1

// Individual unit test toggles
#define DICT_CTOR					1
#define DICT_PAIR_CTOR				1
#define DICT_DTOR					1
#define DICT_CLEAR					1
#define DICT_INSERT					1
#define DICT_INSERT_EXISTING		1 
#define DICT_FIND					1
#define DICT_FIND_NOT_FOUND			1
#define DICT_REMOVE					1
#define DICT_REMOVE_NOT_FOUND		1
#define DICT_ASSIGNMENT_OP			1
#define DICT_COPY_CTOR				1

/************/
/* Includes */
/************/
#include <list>

#if LAB_5

template<typename Key, typename Value>
class Dictionary {

	friend class DSA_TestSuite_Lab5;	// Giving access to test code

	// The objects stored in the hash-table
	struct Pair {
		Key key;			// The key for insertion/lookup
		Value value;		// The data

		// Constructor
		// In:	_key
		//		_value
		Pair(const Key& _key, const Value& _value) {
			
			this->key = _key;
			this->value = _value;
		}

		// For testing
		bool operator==(const Pair& _comp) {
			return (_comp.key == key &&
				_comp.value == value);
		}
	};

	std::list<Pair>* mTable;				// A dynamic array of lists (these are the buckets)
	size_t mNumBuckets;						// Number of elements in mTable
	unsigned int(*mHashFunc)(const Key&);	// Pointer to the hash function


public:

	// Constructor
	// In:	_numBuckets			The number of elements to allocate
	//		_hashFunc			The hashing function to be used
	Dictionary(size_t _numBuckets, unsigned int (*_hashFunc)(const Key&))  {

		this->mNumBuckets = _numBuckets;
		this->mHashFunc = _hashFunc;

		mTable = new list<Pair>[mNumBuckets];
	}

	// Destructor
	//		Cleans up any dynamically allocated memory
	~Dictionary() {
		
		mTable->clear();

		delete[] mTable;
		mTable = nullptr;

		mNumBuckets = NULL;
		mHashFunc = NULL;

	}

	// Copy constructor
	//		Used to initialize one object to another
	// In:	_copy				The object to copy from
	Dictionary(const Dictionary& _copy) {
		
		*this = _copy;

	}

	// Assignment operator
	//		Used to assign one object to another
	// In:	_assign				The object to assign from
	//
	// Return: The invoking object (by reference)
	//		This allows us to daisy-chain
	Dictionary& operator=(const Dictionary& _assign) {

		mHashFunc = _assign.mHashFunc;
		mNumBuckets = _assign.mNumBuckets;

		delete[] mTable;

		mTable = new list<Pair>[mNumBuckets];

		for (int i = 0; i < mNumBuckets; i++)
		{
			for (auto form = _assign.mTable[i].begin(); form != _assign.mTable[i].end(); form++)
			{
				mTable[i].push_back(*form);
			}
		}

		return *this;
	}

	// Clear
	//		Clears all internal data being stored
	//  NOTE:	Does not delete table or reset hash function
	void Clear() {
		
		for (int i = 0; i < mNumBuckets; i++)
		{
			mTable[i].clear();
		}
	}

	// Insert an item into the table
	// In:	_key		The key to add at	
	//		_value		The value at the key
	//
	// NOTE:	If there is already an item at the provided key, overwrite it.
	void Insert(const Key& _key, const Value& _value) {
		
		for (auto form = mTable[mHashFunc(_key)].begin(); form != mTable[mHashFunc(_key)].end(); form++)
		{
			Pair item = *form;

			if (item.key == _key) {
				
				*form = Pair(_key, _value);

			}
		}

		mTable[mHashFunc(_key)].push_back(Pair(_key, _value));
	}

	// Find a value at a specified key
	// In:	_key		The key to search for	
	//
	// Return: A const pointer to the value at the searched key
	// NOTE:		Return a null pointer if key is not present
	const Value* Find(const Key& _key) {
		
		for (auto form = mTable[mHashFunc(_key)].begin(); form != mTable[mHashFunc(_key)].end(); form++)
		{
			Pair item = *form;

			if (item.key == _key) {

				
				return &item.value;
			}

		}

		return nullptr;
	}

	// Remove a value at a specified key
	// In:	_key		The key to remove
	//
	// Return: True, if an item was removed
	bool Remove(const Key& _key) {
		
		for (auto form = mTable[mHashFunc(_key)].begin(); form != mTable[mHashFunc(_key)].end(); form++)
		{
			Pair item = *form;

			if (item.key == _key) {

				mTable[mHashFunc(_key)].erase(form);
				return true;
			}

		}

		return false;
	}
};

#endif	// End LAB_5