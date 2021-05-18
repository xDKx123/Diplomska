#pragma once
#include <fstream>
#include <string>
class BinWriter
{
private:
	std::ofstream input;
	char byte = 0;	//shranimo 1 byte
	int counter = 0;	//stevec bitov

public:
	BinWriter(std::string path);
	~BinWriter();

	char getByte();
	int getCounter();

	void writeBit(bool b);
	void writeByte(char c);
	void writeInt(int i);
	void writeFloat(float f);
};

