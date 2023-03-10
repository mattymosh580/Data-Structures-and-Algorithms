/*
File:			DynArray.h
Author(s):
	Base:		Justin Tackett
				jtackett@fullsail.com
	Student:	

Created:		11.21.2020
Last Modified:	02.26.2021
Purpose:		A dynamically-allocated resizeable array (similar to std::vector)
Notes:			Property of Full Sail University
*/

// Header protection
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
#define LAB_1	1

// Individual unit test toggles
#define LAB1_DYNARRAY_DEFAULT_CTOR						0
#define LAB1_DYNARRAY_CONSTRUCTOR_WITH_ARGS				0
#define LAB1_DYNARRAY_BRACKET_OP_READ_ONLY_BUILT_IN		0
#define LAB1_DYNARRAY_BRACKET_OP_WRITE_BUILT_IN			0
#define LAB1_DYNARRAY_SIZE								0
#define LAB1_DYNARRAY_CAPACITY							0
#define LAB1_DYNARRAY_RESERVE_ZERO						0
#define LAB1_DYNARRAY_RESERVE_DOUBLE					0
#define LAB1_DYNARRAY_RESERVE_VALUE						0
#define LAB1_DYNARRAY_RESERVE_SMALLER					0
#define LAB1_DYNARRAY_APPEND_NO_RESIZE					1
#define LAB1_DYNARRAY_APPEND_RESIZE						1
#define LAB1_DYNARRAY_CLEAR								0
#define LAB1_DYNARRAY_DTOR								0
#define LAB1_DYNARRAY_COPY_CTOR_BUILT_IN				0
#define LAB1_DYNARRAY_COPY_CTOR_CLASS					0
#define LAB1_DYNARRAY_ASSIGNMENT_BUILT_IN				0
#define LAB1_DYNARRAY_ASSIGNMENT_CLASS					0

#if LAB_1

// Our implementation of a vector (simplified)
template<typename Type>
class DynArray {

	// Gives access to test code
	friend class DSA_TestSuite_Lab1;

	// Data members
	Type* mArray;			// The dynamic array that will be managed within the class
	size_t mSize;			// The number of elements currently storing valid data
	size_t mCapacity;		// The total number of elements available in the currently allocated array


	//mSize = current total spots in array filled with numbers
	//mCapacity = Total spots in the the array

public:

	// Default constructor
	//		Creates an empty object
	//		
	// In:	_startingCap		An initial size to start the array at
	//
	// Note: Do not allocate any memory if the _startingCap is 0
	DynArray(size_t _startingCap = 0) {

		if (_startingCap != 0) {

			mArray = new Type[_startingCap];

			this->mCapacity = _startingCap;
		}
	}

	// Destructor
	//		Cleans up all dynamically allocated memory
	~DynArray() {
		
		
		delete[] mArray;
		
	}

	// Copy constructor
	//		Used to initialize one object to another
	// In:	_da				The object to copy from
	DynArray(const DynArray& _copy) : mArray(nullptr) {
		

		mCapacity = _copy.mCapacity;
		mSize = _copy.mSize;
		delete[] mArray;

		mArray = new Type[mCapacity];

		for (int i = 0; i < mCapacity; i++)
		{
			mArray[i] = _copy.mArray[i];
		}
		

	}

	// Assignment operator
	//		Used to assign one object to another
	// In:	_da				The object to assign from
	//
	// Return: The invoking object (by reference)
	//		This allows us to daisy-chain
	DynArray& operator=(const DynArray& _assign) {
		

		mCapacity = _assign.mCapacity;
		mSize = _assign.mSize;
		delete[] mArray;

		mArray = new Type[mCapacity];

		for (int i = 0; i < mCapacity; i++)
		{
			mArray[i] = _assign.mArray[i];

		}

		return *this;
	}

	// Clear
	//		Cleans up all dynamically allocated memory
	//		Sets all data members back to default values
	void Clear() {
		
		delete[] mArray;

		mArray = nullptr;

		this->mCapacity = 0;
	}

	// Overloaded [] operator
	//		Used to access an element in the internal array (read-only)
	// In:	_index			The index to access at
	//
	// Return: The item at the specified index (by reference)
	const Type& operator[](int _index) const {
		
		return mArray[_index];

	}

	// Overloaded [] operator
	//		Used to access an element in the internal array (writeable)
	// In:	_index			The index to access at
	//
	// Return: The item at the specified index (by reference)
	Type& operator[](int _index) {
		

		return mArray[_index];
	}

	// Get the current number of elements actively being used
	//
	// Return: The current number of elements used
	size_t Size() const {
		

		return mSize;
	}

	// Get the current capacity of the internal array
	//
	// Return: The capacity of the array
	size_t Capacity() const {
		

		return mCapacity;

	}

	// Add an item to the end of the array
	//		Should resize the array if needed
	//
	// In:	_data			The item to be added
	void Append(const Type& _data) {
		
		
		if (mSize == mCapacity) {

			Reserve(0);

			mArray[this->mSize] = _data;
		}
		else {
			
			mArray[this->mSize] = _data;
		}

		this->mSize = mSize + 1;

	}

	// Resizes the internal array, and copies all data over
	// In: _newCapacity		The new capacity of the array
	//		NOTE:	If 0 is passed, the array should double in size
	//				If _newCapacity < mCapacity, do nothing
	//
	//	SPECIAL CASE: If mCapacity is 0, then it should be set to 1
	void Reserve(size_t _newCapacity = 0) {
		
		Type* tempArray;

		if (this->mCapacity == 0) {

			this->mCapacity = 1;

			delete[] mArray;

			mArray = new Type[this->mCapacity];

		}
		else if (_newCapacity == 0) {

			this->mCapacity = mCapacity * 2;

			tempArray = new Type[mCapacity];

			for (int i = 0; i < mCapacity; i++)
			{
				tempArray[i] = mArray[i];
			}

			delete[] mArray;

			mArray = new Type[this->mCapacity];

			for (int i = 0; i < mCapacity; i++)
			{
				mArray[i] = tempArray[i];
			}

			delete[] tempArray;
		}

		
	}
};

#endif	// End LAB_1