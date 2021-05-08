#include "PNG_filters.h"
#include <iostream>

PNG_filters::PNG_filters(std::string fileName) : image(cv::imread(fileName)) {}

PNG_filters::~PNG_filters() {}

void PNG_filters::showImage() {
	if (!image.empty()) {
		cv::imshow("Prikaz slike", image);
		cv::waitKey(0);
	}
}

bool PNG_filters::isImage() {
	return !image.empty();
}

short PNG_filters::filterNone() {
	return 0;
}

short PNG_filters::filterSub(short  current, short  left) {
	return (current - left) % 256;
}

short PNG_filters::filterUp(short  current, short  up) {
	return (current-up) % 256;
}

short PNG_filters::filterAverage(short  current, short  left, short  up){
	return ((left + up) / 2) % 256;
}

short PNG_filters::fPaeth(short left, short up, short leftUp) {
	short p = left + up - leftUp;
	short pa = std::abs(p - up);
	short pb = std::abs(p - left);
	short pc = std::abs(p - leftUp);

	if (pa <= pb and pa <= pc) {
		return left;
	}
	if (pb <= pc) {
		return up;
	}
	return leftUp;
}

short PNG_filters::filterPeath(short current, short left, short up, short leftUp){
	return (current - fPaeth(left, up, leftUp)) % 256;
}

std::vector<char>* PNG_filters::Encode() {
	if (!isImage()) {
		return NULL;
	}

	/*std::vector<Values*> *vec = new std::vector<Values*>;*/
	std::vector<char>* vec = new std::vector<char>;

	//cv::Size ms = image.size();
	int height = image.size().height;
	int width = image.size().width;

	int c = 0;

	/*std::vector<short> results(5);*/

	//for (cv::MatIterator_<cv::Vec3b> it = image.begin<cv::Vec3b>(); it != image.end<cv::Vec3b>(); ++it) {
	//	std::cout << (*it)[0] << std::endl;
	//}
	 
	 
	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			//std::cout << image.at(x, y) << std::endl;

			//std::cout << image.at<cv::Vec3b>(x, y) << std::endl;
			cv::Vec3b current = image.at<cv::Vec3b>(x, y);
			cv::Vec3b left;
			cv::Vec3b up;
			cv::Vec3b leftUp;

			left = y - 1 >= 0 ? image.at<cv::Vec3b>(x, y - 1) : NULL;
			up = x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y) : NULL;
			leftUp = y - 1 >= 0 && x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y - 1) : NULL;			
			
			for (int z = 0; z < 3; z++) {
				Values* vl = new Values(current[z]);
				if (left != nullCheck) {
					short fSub = filterSub(current[z], left[z]);
					if (fSub < vl->encodedValue) {
						vl->encodedValue = fSub;
						vl->sf = SelectedFilter::Sub;
					}
				}

				if (up != nullCheck) {
					short fUp = filterUp(current[z], up[z]);
					if (fUp < vl->encodedValue) {
						vl->encodedValue = fUp;
						vl->sf = SelectedFilter::Up;
					}
				}

				if (left != nullCheck and up != nullCheck) {
					short fAvg = filterAverage(current[z], left[z], up[z]);
					if (fAvg < vl->encodedValue) {
						vl->encodedValue = fAvg;
						vl->sf = SelectedFilter::Average;
					}
				}

				if (left != nullCheck and up != nullCheck and leftUp != nullCheck) {
					short fPaeth = filterPeath(current[z], left[z], up[z], leftUp[z]);
					if (fPaeth < vl->encodedValue) {
						vl->encodedValue = fPaeth;
						vl->sf = SelectedFilter::Paeth;
					}
				}

				//vec->push_back(vl);
				
				//std::stringstream ss; 
				//ss << vl->encodedValue;
				vec->push_back(static_cast<char>(vl->sf));
				vec->push_back(static_cast<char>(vl->encodedValue));

				//results.push_back(current[z]);
				//results.push_back(filterSub(current[z], left[z]));
				//results.push_back(filterUp(current[z], up[z]));
				//results.push_back(filterAverage(current[z], left[z], up[z]));
				//results.push_back(filterPeath(current[z], left[z], up[z], leftUp[z]));
				
				//results.clear();
				//std::vector<short>::iterator it = std::min_element(std::begin(results), std::end(results));
				//int smallestIndex = std::distance(std::begin(results), it);
			}
		}
	}

	return vec;
	//std::cout << "imagesize: " << image.size().height << std::endl;
}

