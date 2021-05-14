#include "MTF.h"
#include "Utility.h"

MTF::MTF() : commonDict(Utility::commonDictionaryVector()) {}
MTF::MTF(std::vector<char>* v) : commonDict(v) {}
//MTF::MTF(std::vector<char>* v, std::vector<char>* v1) : input(v), commonDict(v1) {}

MTF::~MTF() {}

/// <summary>
/// Move to front transformacija
/// </summary>
/// <param name="input">vhodni niz znakov</param>
/// <returns>std::vector<int>* - izhod mtf</returns>
std::vector<char>* MTF::Encode(std::vector<char>* input) {
	std::vector<char>* v = new std::vector<char>;
	//int casta v char

	for (auto c : *input) {
		std::vector<char>::iterator it = std::find(commonDict->begin(), commonDict->end(), c);

		//int index = std::distance(commonDict->begin(), it);

		v->push_back(static_cast<char>(std::distance(commonDict->begin(), it)));

		std::rotate(commonDict->begin(), it, it + 1);
		//commonDict->splice(commonDict->begin(),*commonDict, it);
	}

	return v;
	/*std::cout << " " << std::endl;*/
}

/// <summary>
/// Obratna verzija mtf
/// </summary>
/// <param name="input">vhodni niz pozicij</param>
/// <returns>std::vector<char>* - znaki</returns>
std::vector<char>* MTF::Decode(std::vector<char>* input) {
	std::vector<char>* v = new std::vector<char>;

	for (auto i : *input) {
		//char c = commonDict->at(i);
		std::vector<char>::iterator it = std::next(commonDict->begin(), static_cast<int>(i));
		//std::list<char>::iterator c = std::next(commonDict->begin(), i);
		v->push_back(*it);
		//std::list<char>::iterator it = std::find(commonDict->begin(), commonDict->end(), *c);

		std::rotate(commonDict->begin(), it, it + 1);
		//commonDict->splice(commonDict->begin(), *commonDict, it);
	}

	return v;
}