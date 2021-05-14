#include "Huffman.h"
#include "Utility.h"
#include <iostream>
#include <map>
std::vector<bool>* Huffman::Encode(std::vector<char>* v) {
	//class T::value_type s;
	//std::cout << s << std::endl;
	

	std::vector<bool>* b = new std::vector<bool>;

	
	//std::cout << typeid(v).name() << std::endl;

	//if (typeid(v) == typeid(std::vector<char>*)) {

	//}

	std::map<char, int> mp = Utility::commonDictionaryMap();

	for (auto c : *v) {
		//std::map<char, int>::iterator it = std::find(mp.begin(), mp.end(), c);
		mp[c]++;
	}

	return b;
}


std::vector<char>* Huffman::Decode(std::vector<bool>* v) {
	std::vector<char>* b = new std::vector<char>;

	return b;


}