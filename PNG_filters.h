#pragma once
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <string.h>
#include <vector>
#include <optional>

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
	const cv::Vec3b nullCheck = NULL;

	bool isImage();
	void showImage();
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
	PNG_filters();
	PNG_filters(std::string fileName);
	~PNG_filters();

	std::vector<char>* Encode();
	cv::Mat Decode(int width, int height, std::vector<char>* values);

	cv::Size getSize();
	cv::Mat getImage();
};