#pragma once
#include <vector>
#include <list>
class MTF
{
private:
	std::vector<unsigned char> commonDict;

public:
	MTF();
	~MTF();
	std::vector<unsigned char> Encode(std::vector<unsigned char> input);
	std::vector<unsigned char> Decode(std::vector<unsigned char> input);
};

