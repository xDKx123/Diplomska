#include "PNG_filters.h"
#include <iostream>
#include <fstream>

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

int PNG_filters::sumElementsInVector(std::vector<short> v)
{
	return std::accumulate(v.begin(), v.end(), (int)0, [](int acc, const short val) {return acc + std::abs(val); });
}

std::vector<char> PNG_filters::createCompressedVector(std::vector<short> v)
{
	std::vector<char> comp;
	//comp->push_back(static_cast<char> (sf));
	for (auto x : v) {
		comp.push_back(static_cast<char> (x));
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
std::tuple<SelectedFilter, std::vector<short>> PNG_filters::hevristics(std::vector<short> none, std::vector<short> sub, std::vector<short> up, std::vector<short> average, std::vector<short> paeth)
{
	std::vector<int> sums;
	sums.push_back(sumElementsInVector(none));
	sums.push_back(sumElementsInVector(sub));
	sums.push_back(sumElementsInVector(up));
	sums.push_back(sumElementsInVector(average));
	sums.push_back(sumElementsInVector(paeth));

	switch (std::distance(sums.begin(), std::min_element(sums.begin(), sums.end()))) {
		case 0: {
			return std::tuple<SelectedFilter, std::vector<short>>(SelectedFilter::NoneFilter, none);
		}
		case 1: {
			//std::cout << "Filter SUB" << std::endl;
			//currentSelectedFilter = SelectedFilter::Sub;
			//std::cout << "Filter SUb" << std::endl;
			return std::tuple<SelectedFilter, std::vector<short>>(SelectedFilter::Sub, sub);
		}
		case 2: {
			//std::cout << "Filter UP" << std::endl;
			//currentSelectedFilter = SelectedFilter::Up;
			return std::tuple<SelectedFilter, std::vector<short>>(SelectedFilter::Up, up);

		}
		case 3: {
			//std::cout << "Filter AVERAGE" << std::endl;
			//currentSelectedFilter = SelectedFilter::Average;
			return std::tuple<SelectedFilter, std::vector<short>>(SelectedFilter::Average, average);
		}
		case 4: {
			//std::cout << "Filter PEATH" << std::endl;
			//currentSelectedFilter = SelectedFilter::Paeth;
			return std::tuple<SelectedFilter, std::vector<short>>(SelectedFilter::Paeth, paeth);
		}
		default: {
			//std::cout << "Načeloma nikoli ne pride sem, ampak za ziher" << std::endl;
			//currentSelectedFilter = SelectedFilter::Sub;
			return std::tuple<SelectedFilter, std::vector<short>>(SelectedFilter::Sub, sub);

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
	short res = (current - left) % MODULUS;
	return res < 0 ? res + MODULUS : res;
}

short PNG_filters::filterUpEncode(short  current, short  up) {
	short res = (current - up) % MODULUS;
	return res < 0 ? res + MODULUS : res;
}

short PNG_filters::filterAverageEncode(short  current, short  left, short  up){
	short res = (current - static_cast<short>(std::floor(((static_cast<double>(left + up)) / 2)))) % MODULUS;
	return res < 0 ? res + MODULUS : res;
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
	short res = (current - fPaeth(left, up, leftUp)) % MODULUS;
	return res < 0 ? res + MODULUS : res;
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

void PNG_filters::nearLossless(Configuration* configuration)
{
	int height = image.size().height;
	int width = image.size().width;

	//cv::Mat newImage = cv::Mat(cv::Size(width, height), CV_8UC3);
	for (int x = 1; x < height; x++) {
		cv::Vec3b previous = image.at<cv::Vec3b>(x, 0);
		//for (int x = u; x < u + 1 && x < height; x++) {
		for (int y = 1; y < width; y++) {
			cv::Vec3b current = image.at<cv::Vec3b>(x, y);
			cv::Vec3b leftUp = image.at<cv::Vec3b>(x - 1, y - 1);
			cv::Vec3b up = image.at<cv::Vec3b>(x - 1, y);
			//cv::Vec3b rightUp = image.at<cv::Vec3b>(x+1, y);
			cv::Vec3b left = image.at<cv::Vec3b>(x, y - 1);
			//cv::Vec3b right = image.at<cv::Vec3b>(x, y+1);
			//cv::Vec3b downLeft = image.at<cv::Vec3b>(x+1, y-1);
			//cv::Vec3b down = image.at<cv::Vec3b>(x+1, y);
			//cv::Vec3b downRight = image.at<cv::Vec3b>(x+1, y+1);


			cv::Vec3b newPixel;
			for (int z = 0; z < 3; z++) {
				short nearPixels = (leftUp[z] + up[z] + left[z]) / 3;
				short difference = std::abs(current[z] - nearPixels);
				//short difference = std::abs(current[z] - previous[z]);
				if (difference < configuration->getErrorCorrection()) {
					newPixel[z] = nearPixels;
				}
				else {
					newPixel[z] = current[z];
				}
			}

			image.at<cv::Vec3b>(x, y) = newPixel;
			//previous = newPixel;
		}
	}
}

/// <summary>
/// Zakodiramo celotno sliko z izbranim filtrom
/// </summary>
/// <returns>zakodirani podatki</returns>
std::tuple<std::vector<SelectedFilter>, std::vector<short>> PNG_filters::Encode(Configuration* configuration) {
	if (!isImage()) {
		//return NULL;
		throw;
	}

	if (configuration->getErrorCorrection() != 0) {
		nearLossless(configuration);
	}

	int height = image.size().height;
	int width = image.size().width;

	std::vector<SelectedFilter> selFilter;
	std::vector<short> codes;

	std::vector<short> encodedNone;
	std::vector<short> encodedSub;
	std::vector<short> encodedUp;
	std::vector<short> encodedAvg;
	std::vector<short> encodedPaeth;

	/*if (numberOfEncodedRows != NoneRows) {*/
		//int rowNumber = static_cast<int>(numberOfEncodedRows);

	for (int x = 0; x < height; x++) {
		//for (int x = u; x < u + 1 && x < height; x++) {
			for (int y = 0; y < width; y++) {
				cv::Vec3b current = image.at<cv::Vec3b>(x, y);
				cv::Vec3b left;
				cv::Vec3b up;
				cv::Vec3b leftUp;


				left = y - 1 >= 0 ? image.at<cv::Vec3b>(x, y - 1) : NULL;
				up = x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y) : NULL;
				leftUp = y - 1 >= 0 && x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y - 1) : NULL;

				for (int z = 0; z < 3; z++) {
					encodedNone.push_back(current[z]);
					//if (left != nullCheck) {
					//	short fSub = filterSubEncode(current[z], left[z]);

					//	if (fSub >= -errorCor && fSub <= errorCor) {
					//		fSub = 0;
					//	}

					//	encodedSub.push_back(fSub);
					//}
					//else {
					//	encodedSub.push_back(current[z]);
					//}
					encodedSub.push_back(left != nullCheck ? filterSubEncode(current[z], left[z]) : current[z]);

					/*if (up != nullCheck) {
						short fUp = filterUpEncode(current[z], up[z]);

						if (fUp >= -errorCor && fUp <= errorCor) {
							fUp = 0;
						}

						encodedUp.push_back(fUp);
					}
					else {
						encodedUp.push_back(current[z]);
					}*/
					encodedUp.push_back(up != nullCheck ? filterUpEncode(current[z], up[z]) : current[z]);

					/*if (left != nullCheck && up != nullCheck) {


						short fAvg = filterAverageEncode(current[z], left[z], up[z]);

						if (fAvg >= -errorCor && fAvg <= errorCor) {
							fAvg = 0;
						}

						encodedAvg.push_back(fAvg);
					}
					else {
						encodedAvg.push_back(current[z]);
					}*/
					encodedAvg.push_back(left != nullCheck && up != nullCheck ? filterAverageEncode(current[z], left[z], up[z]) : current[z]);
					
					/*if (left != nullCheck && leftUp != nullCheck && up != nullCheck) {
						short fPaeth = filterPeathEncode(current[z], left[z], up[z], leftUp[z]);

						if (fPaeth >= -errorCor && fPaeth <= errorCor) {
							fPaeth = 0;
						}

						encodedPaeth.push_back(fPaeth);
					}
					else {
						encodedPaeth.push_back(current[z]);
					}*/
					encodedPaeth.push_back(left != nullCheck && leftUp != nullCheck && up != nullCheck ? filterPeathEncode(current[z], left[z], up[z], leftUp[z]) : current[z]);
				}
			}
		//}


		SelectedFilter tmpSf;
		std::vector<short> code;

		std::tie(tmpSf, code) = hevristics(encodedNone, encodedSub, encodedUp, encodedAvg, encodedPaeth);

		selFilter.push_back(tmpSf);
		for (auto symbol : code) {
			codes.push_back(symbol);
		}

		encodedSub.clear();
		encodedUp.clear();
		encodedAvg.clear();
		encodedPaeth.clear();

	}

		//return hevristics(encodedSub, encodedUp, encodedAvg, encodedPaeth);

	/*}
	else {
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
					encodedSub.push_back(left != nullCheck ? filterSubEncode(current[z], left[z]) : current[z]);
					encodedUp.push_back(up != nullCheck ? filterUpEncode(current[z], up[z]) : current[z]);
					encodedAvg.push_back(left != nullCheck && up != nullCheck ? filterAverageEncode(current[z], left[z], up[z]) : current[z]);
					encodedPaeth.push_back(left != nullCheck && leftUp != nullCheck && up != nullCheck ? filterPeathEncode(current[z], left[z], up[z], leftUp[z]) : current[z]);
				}
			}
		}

		SelectedFilter tmpSf;
		std::vector<short> code;

		std::tie(tmpSf, code) = hevristics(encodedSub, encodedUp, encodedAvg, encodedPaeth);

		selFilter.push_back(tmpSf);
		for (auto symbol : code) {
			codes.push_back(symbol);
		}
	}*/

	std::ofstream fs;
	fs.open("testing_0.txt");

	int counter = 0;
	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			cv::Vec3b current = image.at<cv::Vec3b>(x, y);
			fs << (short)current[0] << '\t' << codes[counter] << std::endl;
			counter += 3;
		}
		fs << std::endl << std::endl;
	}

	fs.close();

	return std::tuple<std::vector<SelectedFilter>, std::vector<short>>(selFilter, codes);

	//return hevristics(encodedSub, encodedUp, encodedAvg, encodedPaeth);
}

/// <summary>
/// Dekodiramo sliko z izbranim filtrom
/// </summary>
/// <param name="width">�irina slike</param>
/// <param name="height">vi�ina slike</param>
/// <param name="values">slikovni podatki</param>
/// <returns></returns>
cv::Mat PNG_filters::Decode(int width, int height, std::vector<SelectedFilter> selectedFilter, std::vector<unsigned char> values)
{
	//Preverimo �e je koli�ina podatkov za zapis v sliko ustrezna
	if (values.size() != width * height * 3) {
		std::cerr << "Velikost ni pravilna" << std::endl;
		image = NULL;
		return image;
	}
	else {
		std::cout << "Velikost je pravilna" << std::endl;
	}

	int index = 0;
	
	image = cv::Mat(cv::Size(width, height), CV_8UC3);

	//if (numberOfEncodedRows != NoneRows) {
		int heightIndex = 0;
		for (int u = 0; u < selectedFilter.size(); u++, heightIndex ++) {
			for (int x = heightIndex; x < height && x < heightIndex + 1; x++) {
				for (int y = 0; y < width; y++) {
					cv::Vec3b current;
					cv::Vec3b left;
					cv::Vec3b up;
					cv::Vec3b leftUp;

					left = y - 1 >= 0 ? image.at<cv::Vec3b>(x, y - 1) : NULL;
					up = x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y) : NULL;
					leftUp = y - 1 >= 0 && x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y - 1) : NULL;

					for (int z = 0; z < 3; z++, index++) {
						if (selectedFilter.at(u) == Sub) {
							current[z] = left != nullCheck ? filterSubDecode(values.at(index), left[z]) : values.at(index);
						}
						else if (selectedFilter.at(u) == Up) {
							current[z] = up != nullCheck ? filterUpDecode(values.at(index), up[z]) : values.at(index);
						}
						else if (selectedFilter.at(u) == Average) {
							current[z] = left != nullCheck && up != nullCheck ? filterAverageDecode(values.at(index), left[z], up[z]) : values.at(index);
						}
						else if (selectedFilter.at(u) == Paeth)
						{
							current[z] = left != nullCheck && leftUp != nullCheck && up != nullCheck ? filterPaethDecode(values.at(index), left[z], up[z], leftUp[z]) : values.at(index);
						}
					}
					image.at<cv::Vec3b>(x, y) = current;
				}
			}
		}
	/*}
	else {
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
					if (selectedFilter.at(0) == Sub) {
						current[z] = left != nullCheck ? filterSubDecode(values.at(index), left[z]) : values.at(index);
					}
					else if (selectedFilter.at(0) == Up) {
						current[z] = up != nullCheck ? filterUpDecode(values.at(index), up[z]) : values.at(index);
					}
					else if (selectedFilter.at(0) == Average) {
						current[z] = left != nullCheck && up != nullCheck ? filterAverageDecode(values.at(index), left[z], up[z]) : values.at(index);
					}
					else if (selectedFilter.at(0) == Paeth)
					{
						current[z] = left != nullCheck && leftUp != nullCheck && up != nullCheck ? filterPaethDecode(values.at(index), left[z], up[z], leftUp[z]) : values.at(index);
					}
				}

				image.at<cv::Vec3b>(x, y) = current;
			}
		}
	}*/

	std::ofstream fs;
	fs.open("testing_1.txt");

	int counter = 0;
	for (int x = 0; x < height; x++) {
		for (int y = 0; y < width; y++) {
			cv::Vec3b current = image.at<cv::Vec3b>(x, y);
			fs << (short)current[0] << '\t' << (short)values[counter] << std::endl;
			counter += 3;
		}
		fs << std::endl << std::endl;
	}

	fs.close();

	//return std::tuple<std::vector<SelectedFilter>, std::vector<short>>(selFilter, codes);

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
