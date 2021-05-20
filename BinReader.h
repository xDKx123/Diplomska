#pragma once
#include <fstream>
#include <string>
class BinReader
{
private:
	std::ifstream input;
	char byte;
	int counter;

public:
	BinReader(std::string path);
	~BinReader();

	bool isEof();

	bool readBit();
	char readByte();
	int readInt();
	float readFloat();
};

