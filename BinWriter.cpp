#include "BinWriter.h"

BinWriter::BinWriter(std::string path)
{
	input.open(path, std::ios::binary);
}

BinWriter::~BinWriter()
{
	if (counter > 0) {
		//byte = 0;
		input.write(&byte, 1);
	}
	input.close();
}

char BinWriter::getByte()
{
	return byte;
}

int BinWriter::getCounter()
{
	return counter;
}

void BinWriter::writeBit(bool b)
{
	if (counter == 8) {
		input.write(&byte, 1);
		counter = 0;
		byte = 0;
	}
	byte ^= (-b ^ byte) & (1 << counter);
	counter++;
}

void BinWriter::writeByte(char c)
{
	input.write((char*)&c, 1);
}

void BinWriter::writeInt(int i)
{
	input.write((char*)&i, 4);
}

void BinWriter::writeFloat(float f)
{
	input.write((char*)&f, 4);
}
