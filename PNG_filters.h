#pragma once
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <string.h>
#include <vector>
#include <optional>

class PNG_filters
{
private:
	cv::Mat image;
	const cv::Vec3b nullCheck = NULL;
public:
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

	PNG_filters(int) {};
	PNG_filters(std::string fileName);
	~PNG_filters();
	bool isImage();
	void showImage();
	short filterNone();
	short filterSub(short  current, short  left);
	short filterUp(short  current, short  up);
	short filterAverage(short  current, short  left, short  up);
	short fPaeth(short left, short up, short leftUp);
	short filterPeath(short  current, short left, short up, short leftUp);
	std::vector<char>* Encode();

	template<typename T>
	std::vector<int>* runFilterToImage(T function);

};