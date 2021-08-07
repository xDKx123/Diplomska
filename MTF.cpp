#include "MTF.h"
#include "Utility.h"

MTF::MTF() : commonDict(Utility::commonDictionaryVector()) {}
//MTF::MTF(std::vector<char>* v, std::vector<char>* v1) : input(v), commonDict(v1) {}

MTF::~MTF() {}

/// <summary>
/// Move to front transformacija
/// </summary>
/// <param name="input">vhodni niz znakov</param>
/// <returns>std::vector<int>* - izhod mtf</returns>
std::vector<char> MTF::Encode(std::vector<char> input) {
	std::vector<char> v;
	//int casta v char

	for (auto c : input) {
		std::vector<char>::iterator it = std::find(commonDict.begin(), commonDict.end(), c);
		v.push_back(static_cast<char>(std::distance(commonDict.begin(), it)));

		std::rotate(commonDict.begin(), it, it + 1);
	}

	return v;
}

/// <summary>
/// Obratna verzija mtf
/// </summary>
/// <param name="input">vhodni niz pozicij</param>
/// <returns>std::vector<char>* - znaki</returns>
std::vector<char> MTF::Decode(std::vector<char> input) {
	std::vector<char> v;

	for (auto i : input) {
		int position = static_cast<int> (i);

		std::vector<char>::iterator it = std::next(commonDict.begin(), position < 0 ? position + 256 : position);
		v.push_back(*it);

		std::rotate(commonDict.begin(), it, it + 1);
	}

	return v;
}