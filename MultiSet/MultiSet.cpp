#include <iostream>
#include "MultiSet.h"
#include <fstream>

void MultiSet::moveFrom(MultiSet&& other) noexcept
{
	maxNumber = other.maxNumber;
	bits = other.bits;
	dataSize = other.dataSize;
	data = other.data;
	other.data = nullptr;
}
void MultiSet::copyFrom(const MultiSet& other)
{
	maxNumber = other.maxNumber;
	bits = other.bits;
	dataSize = other.dataSize;
	data = new unsigned char[dataSize] {0};
	for (unsigned i = 0;i < dataSize;i++) {
		data[i] = other.data[i];
	}
}
void MultiSet::free()
{
	maxNumber = bits = dataSize = 0;
	delete[] data;
	data = nullptr;
}
unsigned MultiSet::calculateDataSize()
{
	unsigned bitsCount = maxNumber * bits;
	unsigned bytesCount = 1;
	while (bitsCount >= Constants::MAX_BITS*bytesCount) {
		bytesCount++;
	}
	return bytesCount;
}
MultiSet::MultiSet(unsigned _maxNumber, unsigned _bits)
{
	if (_bits > Constants::MAX_BITS) {
		throw std::out_of_range("Bits are out of range!");
	}
	maxNumber = _maxNumber;
	bits = _bits;
	dataSize = calculateDataSize();
	data = new unsigned char[dataSize]();
}
MultiSet::MultiSet(const MultiSet& other)
{
	copyFrom(other);
}
MultiSet::MultiSet(MultiSet&& other) noexcept
{
	moveFrom(std::move(other));
}
MultiSet& MultiSet::operator=(const MultiSet& other)
{
	if (this != &other)
	{
		free();
		copyFrom(other);
	}
	return *this;
}
MultiSet & MultiSet::operator=(MultiSet&& other) noexcept
{
	if (this != &other)
	{
		free();
		moveFrom(std::move(other));
	}
	return *this;
}
MultiSet::~MultiSet()
{
	free();
}
unsigned MultiSet::getfirstByteIndex(unsigned number) const
{
	return (number * bits) / Constants::MAX_BITS;
}
unsigned MultiSet::getFirstBitIndex(unsigned number) const
{
	return (number * bits) % Constants::MAX_BITS;
}
unsigned MultiSet::getNumberCount(unsigned number) const
{
	if (number > maxNumber) {
		throw std::out_of_range("Number is not in range!");
	}
	unsigned count = 0;
	unsigned byteIndex = getfirstByteIndex(number);
	unsigned bitIndex = getFirstBitIndex(number);
	for (int i = bits - 1;i >= 0;i--) {
		if (((data[byteIndex] & (1 << (Constants::MAX_INDEX - bitIndex))) != 0)) {
			count += pow(2, i);
		}
		bitIndex++;
		if (bitIndex >= 8) {
			bitIndex -= 8;
			byteIndex++;
		}
	}
	
	return count;
}
unsigned MultiSet::maxNumberCount() const
{
	return (pow(2,bits)) - 1;
}
void MultiSet::add(unsigned number)
{
	unsigned mask = getNumberCount(number);
	if (mask >= maxNumberCount()) {
		return;
	}
	unsigned byteIndex = getfirstByteIndex(number);
	unsigned bitIndex = getFirstBitIndex(number);
	int shiftIndex = Constants::MAX_BITS - bitIndex - bits;
		if (shiftIndex < 0) {
			data[byteIndex] ^= (mask >> (-shiftIndex));
			data[byteIndex] |= ((mask + 1) >> (-shiftIndex));
			byteIndex++;
			bitIndex = 0;
			data[byteIndex] ^= (mask << (Constants::MAX_BITS + shiftIndex));
			data[byteIndex] |= ((mask + 1) << (Constants::MAX_BITS + shiftIndex));
		}
	data[byteIndex] ^= (mask << (shiftIndex));
	data[byteIndex] |= ((mask + 1) << (shiftIndex));
		
	
}
void MultiSet::addNtimes(unsigned number, unsigned count)
{
	while (getNumberCount(number) <= maxNumberCount() && count>0) {
		add(number);
		count--;
	}
}
void MultiSet::getAllNumbers() const
{
	for (unsigned i = 0;i <= maxNumber;i++) {
		if (getNumberCount(i) > 0) {
			std::cout << i << ' ';
		}
	}
	std::cout << std::endl;
}
void MultiSet::printData() const
{
	for (int i = 0;i <= maxNumber;i++) {
		for (int j = 0;j <= Constants::MAX_INDEX;j++) {
			if((data[i] & (1 << j)))
			{
				std::cout << 1;
			}
			else {
				std::cout << 0;
			}
			
		}
	}
}
void MultiSet::serilization(std::ofstream& ofs)
{
	ofs.write((const char*)&maxNumber, sizeof(maxNumber));
	ofs.write((const char*)&bits, sizeof(bits));
	ofs.write((const char*)&dataSize, sizeof(data));
	ofs.write((const char*)&data, dataSize);
}
void MultiSet::deserilization(std::ifstream& ifs)
{
	
	ifs.read((char*)&maxNumber, sizeof(maxNumber));
	ifs.read((char*)&bits, sizeof(bits));
	ifs.read((char*)&dataSize, sizeof(dataSize));
	data = new unsigned char[dataSize];
	ifs.read((char*)&data, dataSize);
	
}

MultiSet intersectionOfSets(const MultiSet& lhs, const MultiSet& rhs)
{
	MultiSet result( std::min(lhs.maxNumber, rhs.maxNumber), std::min(lhs.bits, rhs.bits));
	for (int i = 0;i <= result.maxNumber;i++) {
		if (lhs.getNumberCount(i) && rhs.getNumberCount(i)) {
			result.add(i);
		}
	}
	return result;
}
MultiSet differenceOfSets(const MultiSet& lhs, const MultiSet& rhs)
{
	MultiSet result(std::min(lhs.maxNumber, rhs.maxNumber), std::min(lhs.bits, rhs.bits));
	for (int i = 0;i <= result.maxNumber;i++) {
		if ((lhs.getNumberCount(i) && !rhs.getNumberCount(i))|| (!lhs.getNumberCount(i) && rhs.getNumberCount(i))) {
			result.add(i);
		}
	}
	return result;
}
MultiSet MultiSet::addition()
{
	MultiSet result(maxNumber, bits);
	for (int i = 0;i <= result.maxNumber;i++) {
		result.addNtimes(i, Constants::MAX_BITS - getNumberCount(i));
	}
	return result;
}
void MultiSet::serilization(const char* fileName)
{
	std::ofstream ofs(fileName, std::ios::binary);
	if (!ofs.is_open()) {
		throw std::logic_error("File not open");
	}
	serilization(ofs);
}
void MultiSet::deserilization(const char* fileName)
{
	std::ifstream ifs(fileName, std::ios::binary);
	if (!ifs.is_open()) {
		throw std::logic_error("File not open");
	}
	deserilization(ifs);
}