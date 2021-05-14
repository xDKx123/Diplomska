#pragma once
#include <vector>
#include <list>
class MTF
{
private:
	std::vector<char>* commonDict;

public:
	MTF();
	MTF(std::vector<char>* v);
	~MTF();
	std::vector<char>* Encode(std::vector<char>* input);
	std::vector<char>* Decode(std::vector<char>* input);
};

