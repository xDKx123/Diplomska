#pragma once
#include <string.h>
#include <vector>
#include <optional>
#include <numeric>
#include <cstdarg>
#include <stdarg.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

#include "SelectedFilter.h"
#include "Configuration.h"

#define MODULUS 256

class PNG_filters
{
private:

	// struct Values {
	// 	SelectedFilter sf;
	// 	char encodedValue;
	// 	//std::vector<short> *values;

	// 	//Values(SelectedFilter sf,std::vector<short>* values) {
	// 	//	this->sf = sf;
	// 	//	this->values = values;
	// 	//}
	// 	//short encodedValue;

	// 	Values(short val) {
	// 		sf = SelectedFilter::NoneFilter;
	// 		encodedValue = val;
	// 	}
	// };

	cv::Mat image;

	bool isImage();
	void showImage();
	int sumElementsInVector(std::vector<short> v);
	std::vector<char> createCompressedVector(std::vector<short> v);
	std::tuple<SelectedFilter, std::vector<short>> hevristics(std::vector<short> none, std::vector<short> sub, std::vector<short> up, std::vector<short> average, std::vector<short> paeth);

	short filterNoneEncode();
	short filterSubEncode(short  current, short  left);
	short filterUpEncode(short  current, short  up);
	short filterAverageEncode(short  current, short  left, short  up);
	short fPaeth(short left, short up, short leftUp);
	short filterPeathEncode(short  current, short left, short up, short leftUp);

	short filterNoneDecode();
	short filterSubDecode(short diff, short left);
	short filterUpDecode(short diff, short up);
	short filterAverageDecode(short diff, short left, short up);
	short filterPaethDecode(short diff, short left, short up, short leftup);

	void nearLossless(Configuration* configuration);
public:
	static inline const cv::Vec3b nullCheck = NULL;
	PNG_filters();
	PNG_filters(std::string fileName);
	~PNG_filters();

	std::tuple<std::vector<SelectedFilter>, std::vector<short>> Encode(Configuration* configuration);
	cv::Mat Decode(int width, int height, std::vector<SelectedFilter> selectedFilter, std::vector<unsigned char> values);

	cv::Size getSize();
	cv::Mat getImage();
};