#include "MTF.h"
#include "Utility.h"

MTF::MTF() : commonDict(Utility::commonDictionary()) {}
MTF::MTF(std::vector<char>* v) : commonDict(v) {}
//MTF::MTF(std::vector<char>* v, std::vector<char>* v1) : input(v), commonDict(v1) {}

MTF::~MTF() {}

std::vector<int>* MTF::Encode(std::vector<char>* input) {
	std::vector<int>* v = new std::vector<int>;

	for (auto c : *input) {
		std::vector<char>::iterator it = std::find(commonDict->begin(), commonDict->end(), c);

		//int index = std::distance(commonDict->begin(), it);

		v->push_back(std::distance(commonDict->begin(), it));

		std::rotate(commonDict->begin(), it, it + 1);
		//commonDict->splice(commonDict->begin(),*commonDict, it);
	}

	return v;
	/*std::cout << " " << std::endl;*/
}

std::vector<char>* MTF::Decode(std::vector<int>* input) {
	std::vector<char>* v = new std::vector<char>;

	for (auto i : *input) {
		//char c = commonDict->at(i);
		std::vector<char>::iterator it = std::next(commonDict->begin(), i);
		//std::list<char>::iterator c = std::next(commonDict->begin(), i);
		v->push_back(*it);
		//std::list<char>::iterator it = std::find(commonDict->begin(), commonDict->end(), *c);

		std::rotate(commonDict->begin(), it, it + 1);
		//commonDict->splice(commonDict->begin(), *commonDict, it);
	}

	return v;
}