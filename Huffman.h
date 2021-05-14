#pragma once
#include <vector>

class Huffman
{
public:
	static std::vector<bool>* Encode(std::vector<char>* v);
	static std::vector<char>* Decode(std::vector<bool>* v);

};

