#include "Huffman.h"
#include "Utility.h"


std::vector<std::pair<char, int>> Huffman::sortedVectorOfValues(std::map<char, int> mp) {
	std::vector<std::pair<char, int>> vec;
	for (std::map<char, int>::iterator it = mp.begin(); it != mp.end(); ++it) {
		if (it->second != 0) {
			vec.push_back(std::pair<char, int>(it->first, it->second));
		}
	}

	std::sort(vec.begin(), vec.end(), [](const std::pair<char, int>& a, const std::pair<char, int>& b) {return a.second < b.second; });

	return vec;
}


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

	//mp.erase(std::remove_if(mp.begin(), mp.end(), [](std::pair<char, int> p) { return p.second != 0; }), mp.end());

	std::vector<std::pair<char, int>> vec = sortedVectorOfValues(mp);

	return b;
}


std::vector<char>* Huffman::Decode(std::vector<bool>* v) {
	std::vector<char>* b = new std::vector<char>;

	return b;


}

