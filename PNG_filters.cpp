#include "PNG_filters.h"
#include <iostream>

PNG_filters::PNG_filters()
{
}

PNG_filters::PNG_filters(std::string fileName) : image(cv::imread(fileName)) {}

PNG_filters::~PNG_filters() {}

void PNG_filters::showImage() {
	if (!image.empty()) {
		cv::imshow("Prikaz slike", image);
		cv::waitKey(0);
	}
}

int PNG_filters::sumElementsInVector(std::vector<short>* v)
{
	return std::accumulate(v->begin(), v->end(), (int)0, [](int acc, const short val) {return acc + std::abs(val); });
}

std::vector<char>* PNG_filters::createCompressedVector(std::vector<short>* v)
{
	std::vector<char>* comp = new std::vector<char>;
	//comp->push_back(static_cast<char> (sf));
	for (auto x : *v) {
		comp->push_back(static_cast<char> (x));
	}
	return comp;
}

/// <summary>
/// Hevristika za ra�unanje najbolj obtimalnega filtra
/// </summary>
/// <param name="sub"></param>
/// <param name="up"></param>
/// <param name="average"></param>
/// <param name="paeth"></param>
/// <returns></returns>
std::vector<char>* PNG_filters::hevristics(std::vector<short>* sub, std::vector<short>* up, std::vector<short>* average, std::vector<short>* paeth)
{
	std::vector<int> sums;
	sums.push_back(sumElementsInVector(sub));
	sums.push_back(sumElementsInVector(up));
	sums.push_back(sumElementsInVector(average));
	sums.push_back(sumElementsInVector(paeth));

	switch (std::distance(sums.begin(), std::min_element(sums.begin(), sums.end()))) {
		case 0: {
			std::cout << "Filter SUB" << std::endl;
			currentSelectedFilter = SelectedFilter::Sub;
			return createCompressedVector(sub);
		}
		case 1: {
			std::cout << "Filter UP" << std::endl;
			currentSelectedFilter = SelectedFilter::Up;
			return createCompressedVector(up);
		}
		case 2: {
			std::cout << "Filter AVERAGE" << std::endl;
			currentSelectedFilter = SelectedFilter::Average;
			return createCompressedVector(average);
		}
		case 3: {
			std::cout << "Filter PEATH" << std::endl;
			currentSelectedFilter = SelectedFilter::Paeth;
			return createCompressedVector(paeth);
		}
		default: {
			std::cout << "Filter default" << std::endl;
			currentSelectedFilter = SelectedFilter::Sub;
			return createCompressedVector(sub);
		}
	}
}

/// <summary>
/// Preverimo �e imamo vne�eno sliko
/// </summary>
/// <returns></returns>
bool PNG_filters::isImage() {
	return !image.empty();
}

short PNG_filters::filterNoneEncode() {
	return 0;
}

short PNG_filters::filterSubEncode(short current, short  left) {
	return (current - left) % MODULUS;
}

short PNG_filters::filterUpEncode(short  current, short  up) {
	return (current-up) % MODULUS;
}

short PNG_filters::filterAverageEncode(short  current, short  left, short  up){
	return (current - static_cast<short>(std::floor(((static_cast<double>(left + up)) / 2)))) % MODULUS;
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

short PNG_filters::filterPeathEncode(short current, short left, short up, short leftUp){
	return (current - fPaeth(left, up, leftUp)) % MODULUS;
}

short PNG_filters::filterNoneDecode()
{
	return 0;
}

short PNG_filters::filterSubDecode(short diff, short left)
{
	return (left + diff) % MODULUS;
}

short PNG_filters::filterUpDecode(short diff, short up)
{
	return (up + diff) % MODULUS;
}

short PNG_filters::filterAverageDecode(short diff, short left, short up)
{
	return (diff + static_cast<short>(std::floor(((static_cast<double>(left + up)) / 2)))) % MODULUS;
}

short PNG_filters::filterPaethDecode(short diff, short left, short up, short leftup)
{
	return (fPaeth(left, up, leftup) + diff) % MODULUS;
}

/// <summary>
/// Zakodiramo celotno sliko z izbranim filtrom
/// </summary>
/// <returns>zakodirani podatki</returns>
std::vector<char>* PNG_filters::Encode() {
	if (!isImage()) {
		return NULL;
	}
	int height = image.size().height;
	int width = image.size().width;

	if (numberOfEncodedRows != NoneRows) {
		std::vector<short> encodedSub;
		std::vector<short> encodedUp;
		std::vector<short> encodedAvg;
		std::vector<short> encodedPaeth;


		int counter = 0;
		for (int x = 0; x < static_cast<int>(numberOfEncodedRows) && x < height; x++, counter+=numberOfEncodedRows) {
			for (int y = 0; y < width; y++) {
				cv::Vec3b current = image.at<cv::Vec3b>(x, y);
				cv::Vec3b left;
				cv::Vec3b up;
				cv::Vec3b leftUp;

				left = y - 1 >= 0 ? image.at<cv::Vec3b>(x, y - 1) : NULL;
				up = x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y) : NULL;
				leftUp = y - 1 >= 0 && x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y - 1) : NULL;

				for (int z = 0; z < 3; z++) {
					encodedSub.push_back(left != nullCheck ? filterSubEncode(current[z], left[z]) : current[z]);
					encodedUp.push_back(up != nullCheck ? filterUpEncode(current[z], up[z]) : current[z]);
					encodedAvg.push_back(left != nullCheck && up != nullCheck ? filterAverageEncode(current[z], left[z], up[z]) : current[z]);
					encodedPaeth.push_back(left != nullCheck && leftUp != nullCheck && up != nullCheck ? filterPeathEncode(current[z], left[z], up[z], leftUp[z]) : current[z]);
				}
			}
		}

		return hevristics(encodedSub, encodedUp, encodedAvg, encodedPaeth);

	}

	//std::vector<Values> encodedValues;
	std::vector<short>* encodedSub = new std::vector<short>;
	std::vector<short>* encodedUp = new std::vector<short>;
	std::vector<short>* encodedAvg = new std::vector<short>;
	std::vector<short>* encodedPaeth = new std::vector<short>;
	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			cv::Vec3b current = image.at<cv::Vec3b>(x, y);
			cv::Vec3b left;
			cv::Vec3b up;
			cv::Vec3b leftUp;

			left = y - 1 >= 0 ? image.at<cv::Vec3b>(x, y - 1) : NULL;
			up = x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y) : NULL;
			leftUp = y - 1 >= 0 && x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y - 1) : NULL;

			for (int z = 0; z < 3; z++) {
				encodedSub->push_back(left != nullCheck ? filterSubEncode(current[z], left[z]) : current[z]);
				encodedUp->push_back(up != nullCheck ? filterUpEncode(current[z], up[z]) : current[z]);
				encodedAvg->push_back(left != nullCheck && up != nullCheck ? filterAverageEncode(current[z], left[z], up[z]) : current[z]);
				encodedPaeth->push_back(left != nullCheck && leftUp != nullCheck && up != nullCheck ? filterPeathEncode(current[z], left[z], up[z], leftUp[z]) : current[z]);
			}
		}
	}

	return hevristics(encodedSub, encodedUp, encodedAvg, encodedPaeth);
}

/// <summary>
/// Dekodiramo sliko z izbranim filtrom
/// </summary>
/// <param name="width">�irina slike</param>
/// <param name="height">vi�ina slike</param>
/// <param name="values">slikovni podatki</param>
/// <returns></returns>
cv::Mat PNG_filters::Decode(int width, int height, std::vector<char>* values)
{
	//Preverimo �e je koli�ina podatkov za zapis v sliko ustrezna
	if (values->size() != width * height * 3) {
		std::cerr << "Velikost ni pravilna" << std::endl;
		image = NULL;
		return image;
	}
	else {
		std::cout << "Velikost je pravilna" << std::endl;
	}

	int index = 0;
	
	image = cv::Mat(cv::Size(width, height), CV_8UC3);

	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			cv::Vec3b current;
			cv::Vec3b left;
			cv::Vec3b up;
			cv::Vec3b leftUp;

			left = y - 1 >= 0 ? image.at<cv::Vec3b>(x, y - 1) : NULL;
			up = x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y) : NULL;
			leftUp = y - 1 >= 0 && x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y - 1) : NULL;

			for (int z = 0; z < 3; z++, index++) {
				if (currentSelectedFilter == Sub) {
					current[z] = left != nullCheck ? filterSubDecode(values->at(index), left[z]) : values->at(index);
				}
				else if (currentSelectedFilter == Up) {
					current[z] = up != nullCheck ? filterUpDecode(values->at(index), up[z]) : values->at(index);
				}
				else if (currentSelectedFilter == Average) {
					current[z] =left != nullCheck && up != nullCheck ? filterAverageDecode(values->at(index), left[z], up[z]) : values->at(index);
				}
				else if (currentSelectedFilter == Paeth)
				{
					current[z] = left != nullCheck && leftUp != nullCheck && up != nullCheck ? filterPaethDecode(values->at(index), left[z], up[z], leftUp[z]) : values->at(index);
				}
			}

			image.at<cv::Vec3b>(x, y) = current;
		}
	}

	return image;
}

cv::Size PNG_filters::getSize()
{
	return image.size();
}

cv::Mat PNG_filters::getImage()
{
	return image;
}
