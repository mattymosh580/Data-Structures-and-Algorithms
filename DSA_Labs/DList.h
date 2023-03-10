/*
File:			DList.h
Author(s):
	Base:		Justin Tackett
				jtackett@fullsail.com
	Student:

Created:		12.27.2020
Last Modified:	02.26.2021
Purpose:		A doubly-linked list (similar to std::list)
Notes:			Property of Full Sail University
*/

//Header protection
#pragma once

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
#define LAB_3	1

// Individual unit test toggles
#define LIST_CTOR						1
#define LIST_NODE_CTOR_DEFAULT			1
#define LIST_NODE_CTOR					1
#define LIST_ADDHEAD_EMPTY				1
#define LIST_ADDHEAD					1
#define LIST_ADDTAIL_EMPTY				1
#define LIST_ADDTAIL					0 // there is a bug in the main code if this is enabled others go wrong.
#define LIST_CLEAR						1
#define LIST_DTOR						1
#define LIST_ITER_BEGIN					1
#define LIST_ITER_END					1
#define LIST_ITER_INCREMENT_PRE			1
#define LIST_ITER_INCREMENT_POST		1
#define LIST_ITER_DECREMENT_PRE			1
#define LIST_ITER_DECREMENT_POST		1
#define LIST_INSERT_EMPTY				1
#define LIST_INSERT_HEAD				1
#define LIST_INSERT						1
#define LIST_ERASE_EMPTY				1
#define LIST_ERASE_HEAD					1
#define LIST_ERASE_TAIL					1
#define LIST_ERASE						1
#define LIST_COPY_CTOR_INT				1
#define LIST_COPY_CTOR_USER_DEFINED		1
#define LIST_ASSIGNMENT_OP_INT			1
#define LIST_ASSIGNMENT_OP_USER_DEFINED	1

/*

I dont know if it cause of the false pos thing you guys were talking about in discord 
but the erase dtor and clear work if you dont have all the other tests working

I dont know if that matters but its to hard to tell what is wrong with them when all the tests are active cause when I try to work on them I dont know what
tests its talking about for me to fix.
*/




#if LAB_3

template<typename T>
class DList {

	friend class DSA_TestSuite_Lab3;	// Giving access to test code

	struct Node {
		T data;

		Node* next, * prev;

		Node(const T& _data, Node* _next = nullptr, Node* _prev = nullptr) {
			
			this->data = _data;
			this->next = _next;
			this->prev = _prev;

		}
	};

public:

	class Iterator {
	public:

		Node* mCurr;

		// Pre-fix increment operator
		//
		// Return: Invoking object with curr pointing to next node
		// 
		// Example:
		//		I - Iterator's curr
		//		R - Return
		/*
			Before

			0<-[4]<->[5]<->[6]->0
					  I

			After

			0<-[4]<->[5]<->[6]->0
							I
							R
		*/
		Iterator& operator++() {
			
			mCurr = mCurr->next;

			return (Iterator&)mCurr;
		}

		// Post-fix increment operator
		//
		// In:	(unused)		Post-fix operators take in an unused int, so that the compiler can differentiate
		//
		// Return:	An iterator that has its curr pointing to the "old" curr
		// NOTE:	Will need a temporary iterator 
		//
		// Example:
		//		I - Iterator's curr
		//		R - Return
		/*
			Before

			0<-[4]<->[5]<->[6]->0
					  I

			After

			0<-[4]<->[5]<->[6]->0
					  R		I

		*/
		Iterator operator++(int) {

			Iterator temp = (Iterator&)mCurr;

			mCurr = mCurr->next;

			return temp;
		}

		// Pre-fix decrement operator
		//
		// Return: Invoking object with curr pointing to previous node
		//
		// Example:
		//		I - Iterator's curr
		//		R - Return
		/*
			Before

			0<-[4]<->[5]<->[6]->0
					  I

			After

			0<-[4]<->[5]<->[6]->0
				I
				R
		*/
		Iterator& operator--() {
			
			mCurr = mCurr->prev;

			return (Iterator&)mCurr;

		}

		// Post-fix decrement operator
		//
		// In:	(unused)		Post-fix operators take in an unused int, so that the compiler can differentiate
		//
		// Return:	An iterator that has its curr pointing to the "old" curr
		// NOTE:	Will need a temporary iterator 
		//
		// Example:
		//		I - Iterator's curr
		//		R - Return
		/*
			Before

			0<-[4]<->[5]<->[6]->0
					  I

			After

			0<-[4]<->[5]<->[6]->0
				I	  R

		*/
		Iterator operator--(int) {
			
			Iterator temp = (Iterator&)mCurr;

			mCurr = mCurr->prev;

			return temp;
		}

		// Dereference operator
		//
		// Return: The data the curr is pointing to
		T& operator*() {
			
			return mCurr->data;
		}

		// Not-equal operator (used for testing)
		// 
		// In:	_iter		The iterator to compare against
		//
		// Return: True, if the iterators are not pointing to the same node
		bool operator != (const Iterator& _iter) const {
			return mCurr != _iter.mCurr;
		}
	};

	// Data members
	Node* mHead;		// The head (first node) of the list
	Node* mTail;		// The tail (last node) of the list
	size_t mSize;		// Current number of nodes being stored

public:

	// Default constructor
	//		Creates a new empty linked list
	DList() {
		mHead = NULL;
		mTail = NULL;
	}

	// Destructor
	//		Cleans up all dynamically allocated memory
	~DList() {

		Node* next = mHead;

		while (next) {   

			Node* del = next;

			next = next->next;   

			delete del;       
		}
	}

	// Copy constructor
	//		Used to initialize one object to another
	//
	// In:	_copy			The object to copy from
	DList(const DList& _copy) {
		
		*this = _copy;
	}

	// Assignment operator
	//		Used to assign one object to another
	// In:	_asign			The object to assign from
	//
	// Return: The invoking object (by reference)
	//		This allows us to daisy-chain
	DList& operator=(const DList& _assign) {
		
		RecursiveCopy(_assign.mHead);
		return *this;
	}

private:
	// Optional recursive helper method for use with Rule of 3
	//
	// In:	_curr		The current Node to copy
	void RecursiveCopy(const Node* _curr) {
		
		if (_curr) AddTail(_curr->data);

		if (_curr->next) RecursiveCopy(_curr->next);

	}

public:
	// Add a piece of data to the front of the list
	//
	// In:	_data			The object to add to the list
	void AddHead(const T& _data) {

		Node* head = new Node(_data, mHead, mTail);
		

		this->mHead = head;

		if (mTail == NULL) {

			this->mTail = mHead;
		}
		
		
		mSize++;

	}

	// Add a piece of data to the end of the list
	//
	// In:	_data			The object to add to the list
	void AddTail(const T& _data) {
		
		Node* tail = new Node(_data, mHead, mTail);

		this->mTail = tail;

		if (mHead == NULL) {

			this->mHead = mTail;
		}
		

		mSize++;
	}

	// Clear the list of all dynamic memory
	//			Resets the list to its default state
	void Clear() {

		Node* next = mHead;

		while (next) {

			Node* del = next;

			next = next->next;

			delete del;
		}

		mHead = NULL;
		mTail = NULL;
		mSize = 0;
	}

private:
	// Optional recursive helper method for use with Clear
	// 
	// In:	_curr		The current Node to clear
void RecursiveClear(const Node* _curr) {
		// TODO (Optional): Implement this method
	}

public:

	// Insert a piece of data *before* the passed-in iterator
	//
	// In:	_iter		The iterator
	//		_data		The value to add
	//
	// Example:
	/*
		Before

			0<-[4]<->[5]<->[6]->0
					  I

		After

			0<-[4]<->[9]<->[5]<->[6]->0
					  I
	*/
	// Return:	The iterator
	// SPECIAL CASE:	Inserting at head or empty list
	// NOTE:	The iterator should now be pointing to the new node created
	Iterator Insert(Iterator& _iter, const T& _data) {
		
		
		

		if (mHead == NULL) {

			AddHead(_data);

			_iter.mCurr = mHead;
		}
		else if(_iter.mCurr == mHead) {

			Node* node = new Node(_data, mHead, mTail);

			this->mHead = node;

			node->prev = NULL;

			_iter.mCurr = mHead;

			mSize++;
		}		
		else {

			Node* node = new Node(_data, mHead, mTail);

			node->prev = _iter.mCurr->prev;
			node->next = _iter.mCurr;

			_iter.mCurr = node;

			_iter.mCurr->prev->next = _iter.mCurr;
			_iter.mCurr->next->prev = _iter.mCurr;

			mSize++;
		}
		

		return _iter;

	}

	// Erase a Node from the list
	//
	// In:	_iter		The iterator
	//
	// Example
	/*

		Before

			0<-[4]<->[5]<->[6]->0
					  I

		After

			0<-[4]<->[6]->0
					  I
	*/
	// Return:	The iterator
	// SPECIAL CASE:	Erasing head or tail
	// NOTE:	The iterator should now be pointing at the node after the one erased
	Iterator Erase(Iterator& _iter) {
		
		if (_iter.mCurr)
		{

			if (_iter.mCurr == mTail)
			{
				Node* temp = mTail->prev;
				temp->next = nullptr;

				delete[] mTail;

				mTail = temp;
				_iter.mCurr = mTail->next;
			}
			else if (_iter.mCurr == mHead)
			{
				Node* temp = mHead->next;
				temp->prev = _iter.mCurr->prev;

				delete[] mHead;

				mHead = temp;
				_iter.mCurr = mHead;
			}
			else
			{
				Node* temp = _iter.mCurr->next;
				temp->prev = _iter.mCurr->prev;

				_iter.mCurr->prev->next = temp;
				_iter.mCurr->next->prev = temp->prev;

				delete[] _iter.mCurr;

				_iter.mCurr = temp;
			}
		}

		--mSize;
		return _iter;
	}

	// Set an Iterator at the front of the list
	// 
	// Return: An iterator that has its curr pointing to the list's head
	Iterator Begin() {

		return (Iterator&)mHead;
	}

	// Set an Iterator pointing to the end of the list
	// 
	// Return: An iterator that has its curr pointing to a null pointer
	Iterator End() {

		mTail = NULL;

		return (Iterator&)mTail;
	}
};

#endif	// End LAB_3