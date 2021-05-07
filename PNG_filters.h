#pragma once
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <string.h>
#include <vector>

class PNG_filters
{
private:
	cv::Mat image;
	const cv::Vec3b nullCheck = NULL;
public:
	static enum class SelectedFilter
	{
		None, Sub, Up, Average, Paeth
	};

	static struct Values {
		SelectedFilter sf;
		short encodedValue;

		Values(short val) {
			sf = SelectedFilter::None;
			encodedValue = val;
		}
	};

	PNG_filters(std::string fileName);
	bool isImage();
	void showImage();
	short filterNone();
	short filterSub(short  current, short  left);
	short filterUp(short  current, short  up);
	short filterAverage(short  current, short  left, short  up);
	short fPaeth(short left, short up, short leftUp);
	short filterPeath(short  current, short left, short up, short leftUp);
	std::vector<Values*>* algorithm();

};

