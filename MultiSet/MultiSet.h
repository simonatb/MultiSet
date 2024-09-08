#pragma once
#include <exception>
#include <iostream>


namespace Constants {
	constexpr unsigned MAX_BITS = 8;
	constexpr unsigned MAX_INDEX = 7;
}

class MultiSet {

	unsigned char* data = nullptr;
	unsigned maxNumber = 0;
	unsigned bits = 0;
	unsigned dataSize = 0;

	void moveFrom(MultiSet&& other) noexcept;
	void copyFrom(const MultiSet& other);
	void free();

	//Helper for the parametric constructor, not to be used by users
	unsigned calculateDataSize();
	
public:
	
	//Big six
	MultiSet() = default;
	MultiSet(unsigned _maxNumber, unsigned _bits);
	MultiSet(const MultiSet& other);
	MultiSet(MultiSet&& other) noexcept;
	MultiSet& operator=(const MultiSet& other);
	MultiSet& operator=(MultiSet&& other) noexcept;
	~MultiSet();

	//Functions for indexes
	unsigned getfirstByteIndex(unsigned number) const;
	unsigned getFirstBitIndex(unsigned number) const;
	unsigned getNumberCount(unsigned number) const;
	unsigned maxNumberCount() const;

	//bitwise operations
	void add(unsigned number);
	void addNtimes(unsigned number, unsigned count);
	void getAllNumbers() const;
	void printData() const;

	//Files with stream or filename
	void serilization(std::ofstream& ofs);
	void serilization(const char* fileName);
	void deserilization(const char* fileName);
	void deserilization(std::ifstream& ifs);

	//Sets
	friend MultiSet intersectionOfSets(const MultiSet& lhs, const MultiSet& rhs);
	friend MultiSet differenceOfSets(const MultiSet& lhs, const MultiSet& rhs);
	MultiSet addition();
};