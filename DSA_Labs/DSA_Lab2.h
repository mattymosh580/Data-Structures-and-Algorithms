/*
File:			DSA_Lab2.h
Author(s):
	Base:		Justin Tackett
				jtackett@fullsail.com
	Student:

Created:		11.30.2020
Last Modified:	02.26.2021
Purpose:		Usage of the std::vector class
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
#define LAB_2	1

// Individual unit test toggles
#define LAB2_PALINDROME_NUMBER			1
#define LAB2_VECTOR_FILL_FILE			1
#define LAB2_VECTOR_FILL_ARRAY			1
#define LAB2_VECTOR_CLEAR				1
#define LAB2_VECTOR_SORT_ASCENDING		1
#define LAB2_VECTOR_SORT_DESCENDING		1
#define LAB2_VECTOR_BRACKETS			1
#define LAB2_VECTOR_CONTAINS_TRUE		1
#define LAB2_VECTOR_CONTAINS_FALSE		1
#define LAB2_VECTOR_MOVE_PALINDROME		1

/************/
/* Includes */
/************/
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

#if LAB_2

// Checks to see if a number is a palindrome (reads the same forwards and backwards)
//		An example of a palindrome word would be "racecar"
//
// In: _num			The number to check
//
// Return: True, if the number is a palindrome number
inline bool IsPalindromeNumber(unsigned int _num) {

	int backwardsNum = 0, mainNum = _num;

	while (_num > 0) {

		backwardsNum = backwardsNum * 10 + _num % 10;

		_num = _num / 10;
	}

	if (backwardsNum == mainNum) {
		return true;
	}
	else return false;
}

class DSA_Lab2
{
	friend class DSA_TestSuite_Lab2;	// Giving access to test code

private:

	std::vector<unsigned int> mValues;		// contains all of the values
	std::vector<unsigned int> mPalindromes;	// contains just the numbers that are palindromes (only used in MovePalindromes method)

public:

	// Fill out the mValues vector with the contents of the binary file
	//		First four bytes of the file are the number of ints in the file
	//
	// In:	_input		Name of the file to open
	void Fill(const char* _input) {
		
		int temp = 0;
		unsigned int fileSize = 0;

		ifstream openFile(_input, ios::binary | ios::in);

		openFile.read((char*)&fileSize, sizeof(int));

        for (int i = 0; i < fileSize; i++)
        {
			openFile.read((char*)&temp, sizeof(int));

            mValues.push_back(temp);
        }

		openFile.close();
	}

	// Fill out the mValues vector with the contents of an array
	//
	// In:	_arr			The array of values
	//		_size			The number of elements in the array
	void Fill(const unsigned int* _arr, size_t _size) {
		
		for (int i = 0; i < _size; i++)
		{
			mValues.push_back(_arr[i]);
		}
	}

	// Remove all elements from vector and decrease capacity to 0
	void Clear() {
		
		mValues.erase(mValues.begin(), mValues.end());

		mValues.shrink_to_fit();
	}

	// Sort the vector 
	//
	// In:	_ascending		To sort in ascending order or not
	//
	// NOTE: Use the std::sort method in this implementation
	void Sort(bool _ascending) {
		
		if (_ascending) {

			sort(mValues.begin(), mValues.end());

		}
		else sort(mValues.begin(), mValues.end(), greater<int>());

	}

	// Get an individual element from the mValues vector
	int operator[](int _index) {
		
		return mValues[_index];
	}

	// Determine if a value is present in the vector
	// 
	// In:	_val		The value to check for
	//
	// Return: True, if the value is present
	bool Contains(unsigned int _val) const {
		
		if (count(mValues.begin(), mValues.end(), _val)) return true;

		else return false;

	}

	// Move all palindrome numbers from mValues vector to mPalindromes vector
	//
	// Pseudocode:
	//		iterate through the main values vector
	//			if the value is a palindrome
	//				add it to the palindrome vector
	//				remove it from the values vector
	void MovePalindromes() {

		for (int i = 0; i < mValues.size(); i++)
		{
			
			if (IsPalindromeNumber(mValues.at(i))) {

				mPalindromes.push_back(mValues.at(i));

				mValues.erase(mValues.begin() + i);

				i--;
			}
			
		}

	}
};

#endif	// End LAB_2