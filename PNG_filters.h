#pragma once
#include <opencv4/opencv2/imgcodecs.hpp>
#include <opencv4/opencv2/highgui.hpp>
#include <string.h>
#include <vector>
#include <optional>
#include <numeric>
#include <cstdarg>
#include <stdarg.h>

#define MODULUS 256

class PNG_filters
{
private:
	enum class SelectedFilter
	{
		None, Sub, Up, Average, Paeth
	};

	struct Values {
		SelectedFilter sf;
		char encodedValue;
		//std::vector<short> *values;

		//Values(SelectedFilter sf,std::vector<short>* values) {
		//	this->sf = sf;
		//	this->values = values;
		//}
		//short encodedValue;

		Values(short val) {
			sf = SelectedFilter::None;
			encodedValue = val;
		}
	};

	cv::Mat image;

	bool isImage();
	void showImage();
	int sumElementsInVector(std::vector<short>* v);
	std::vector<char>* createCompressedVector(SelectedFilter sf, std::vector<short>* v);
	std::vector<char>* hevristics(std::vector<short>* sub, std::vector<short>* up, std::vector<short>* average, std::vector<short>* paeth);

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
public:
	static inline const cv::Vec3b nullCheck = NULL;
	PNG_filters();
	PNG_filters(std::string fileName);
	~PNG_filters();

	std::vector<char>* Encode();
	cv::Mat Decode(int width, int height, std::vector<char>* values);

	cv::Size getSize();
	cv::Mat getImage();
};