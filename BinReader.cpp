#include "BinReader.h"

BinReader::BinReader(std::string path)
{
	input.open(path, std::ios::binary);
	byte = 0;
	counter = 0;
}

BinReader::~BinReader()
{
	input.close();
}

bool BinReader::readBit() {
	if (counter == 8) {
		input.read((char*)&byte, 1);
		counter = 0;
	}

	bool b = (byte >> counter) & 1;
	counter++;
	return b;
}

char BinReader::readByte() {
	input.read((char*)&byte, 1);
	byte = 0;
	return byte;
}

bool BinReader::isEof()
{
	return input.eof();
}

int BinReader::readInt() {
	int i;
	input.read((char*)&i, 4);
	byte = 0;
	return i;
}

float BinReader::readFloat() {
	float f;
	input.read((char*)&f, 4);
	byte = 0;
	return f;
}