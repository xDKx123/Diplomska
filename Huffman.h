#pragma once
#include <vector>
#include <iostream>
#include <map>
#include <set>

class Huffman
{
private:
	std::vector<std::pair<char, int>> sortedVectorOfValues(std::map<char, int> mp);

public:
	std::vector<bool>* Encode(std::vector<char>* v);
	std::vector<char>* Decode(std::vector<bool>* v);


};

