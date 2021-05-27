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

std::vector<char>* PNG_filters::createCompressedVector(SelectedFilter sf, std::vector<short>* v)
{
	std::vector<char>* comp = new std::vector<char>;
	comp->push_back(static_cast<char> (sf));
	for (auto x : *v) {
		comp->push_back(static_cast<char> (x));
	}
	return nullptr;
}

/// <summary>
/// Hevristika za raèunanje najbolj obtimalnega filtra
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
			return createCompressedVector(SelectedFilter::Sub, sub);
		}
		case 1: {
			return createCompressedVector(SelectedFilter::Up, up);
		}
		case 2: {
			return createCompressedVector(SelectedFilter::Average, average);
		}
		case 3: {
			return createCompressedVector(SelectedFilter::Paeth, paeth);
		}
	}
}

/// <summary>
/// Preverimo èe imamo vnešeno sliko
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

	/*std::vector<Values*> *vec = new std::vector<Values*>;*/
	//std::vector<char>* vec = new std::vector<char>;

	//cv::Size ms = image.size();
	int height = image.size().height;
	int width = image.size().width;

	/*std::vector<short> results(5);*/

	//for (cv::MatIterator_<cv::Vec3b> it = image.begin<cv::Vec3b>(); it != image.end<cv::Vec3b>(); ++it) {
	//	std::cout << (*it)[0] << std::endl;
	//}
	 





	//èez celotno sliko uporabi filter in vrstice ter primerjaj rezultate.



	//for (int x = 0; x < height; x++) {
	//	for (int y = 0; y < width; y++) {
	//		//std::cout << image.at(x, y) << std::endl;

	//		//std::cout << image.at<cv::Vec3b>(x, y) << std::endl;
	//		cv::Vec3b current = image.at<cv::Vec3b>(x, y);
	//		cv::Vec3b left;
	//		cv::Vec3b up;
	//		cv::Vec3b leftUp;

	//		left = y - 1 >= 0 ? image.at<cv::Vec3b>(x, y - 1) : NULL;
	//		up = x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y) : NULL;
	//		leftUp = y - 1 >= 0 && x - 1 >= 0 ? image.at<cv::Vec3b>(x - 1, y - 1) : NULL;			
	//		
	//		for (int z = 0; z < 3; z++) {
	//			Values* vl = new Values(current[z]);
	//			if (left != nullCheck) {
	//				short fSub = filterSub(current[z], left[z]);
	//				if (fSub < vl->encodedValue) {
	//					vl->encodedValue = fSub;
	//					vl->sf = SelectedFilter::Sub;
	//				}
	//			}

	//			if (up != nullCheck) {
	//				short fUp = filterUp(current[z], up[z]);
	//				if (fUp < vl->encodedValue) {
	//					vl->encodedValue = fUp;
	//					vl->sf = SelectedFilter::Up;
	//				}
	//			}

	//			if (left != nullCheck and up != nullCheck) {
	//				short fAvg = filterAverage(current[z], left[z], up[z]);
	//				if (fAvg < vl->encodedValue) {
	//					vl->encodedValue = fAvg;
	//					vl->sf = SelectedFilter::Average;
	//				}
	//			}

	//			if (left != nullCheck and up != nullCheck and leftUp != nullCheck) {
	//				short fPaeth = filterPeath(current[z], left[z], up[z], leftUp[z]);
	//				if (fPaeth < vl->encodedValue) {
	//					vl->encodedValue = fPaeth;
	//					vl->sf = SelectedFilter::Paeth;
	//				}
	//			}

	//			//vec->push_back(vl);
	//			
	//			//std::stringstream ss; 
	//			//ss << vl->encodedValue;
	//			vTesting.push_back(static_cast<char>(vl->sf));
	//			vTesting.push_back(static_cast<char>(vl->encodedValue));
	//			//vec->push_back(static_cast<char>(vl->sf));
	//			//vec->push_back(static_cast<char>(vl->encodedValue));

	//			//results.push_back(current[z]);
	//			//results.push_back(filterSub(current[z], left[z]));
	//			//results.push_back(filterUp(current[z], up[z]));
	//			//results.push_back(filterAverage(current[z], left[z], up[z]));
	//			//results.push_back(filterPeath(current[z], left[z], up[z], leftUp[z]));
	//			
	//			//results.clear();
	//			//std::vector<short>::iterator it = std::min_element(std::begin(results), std::end(results));
	//			//int smallestIndex = std::distance(std::begin(results), it);
	//		}
	//	}
	//}


	std::vector<Values> encodedValues;
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

	//if (smallest == 0) {
	//	vec->push_back(static_cast<char> (SelectedFilter::Sub));
	//	for (auto x : *encodedSub) {
	//		vec->push_back(static_cast<char> (x));
	//	}
	//}
	//else if (smallest == 1) {
	//	vec->push_back(static_cast<char> (SelectedFilter::Up));
	//	for (auto x : *encodedUp) {
	//		vec->push_back(static_cast<char> (x));
	//	}
	//}
	//else if (smallest == 2) {
	//	vec->push_back(static_cast<char> (SelectedFilter::Average));
	//	for (auto x : *encodedAvg) {
	//		vec->push_back(static_cast<char> (x));
	//	}
	//}
	//else {
	//	vec->push_back(static_cast<char> (SelectedFilter::Paeth));
	//	for (auto x : *encodedPaeth) {
	//		vec->push_back(static_cast<char> (x));
	//	}
	//}

	//std::cout << "Vec size: " << vec->size() << std::endl;
	//return vec;
	//std::cout << "imagesize: " << image.size().height << std::endl;
}

/// <summary>
/// Dekodiramo sliko z izbranim filtrom
/// </summary>
/// <param name="width">širina slike</param>
/// <param name="height">višina slike</param>
/// <param name="values">slikovni podatki</param>
/// <returns></returns>
cv::Mat PNG_filters::Decode(int width, int height, std::vector<char>* values)
{
	//Preverimo èe je kolièina podatkov za zapis v sliko ustrezna
	if (values->size() - 1 != width * height * 3) {
		std::cerr << "Velikost ni pravilna" << std::endl;
		image = NULL;
		return image;
	}
	else {
		std::cout << "Velikost je pravilna" << std::endl;
	}

	int index = 1;
	int encoded = static_cast<int>(values->at(0));

	//Preverimo, èe je pravilno nastavljen tip filtriranja slike.
	if (encoded < 1 or encoded > 4) {
		std::cerr << "Tip kodiranja ni doloèen pravilno" << std::endl;
		image = NULL;
		return image;
	}

	//image = cv::Mat(width, height, CV_8UC3);
	image = cv::Mat(cv::Size(width, height), CV_8UC3);

	//cv::Vec3b current;
	//image.push_back(current);
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
				if (encoded == static_cast<int>(SelectedFilter::Sub)) {
					current[z] = left != nullCheck ? filterSubDecode(values->at(index), left[z]) : values->at(index);
				}
				else if (encoded == static_cast<int>(SelectedFilter::Up)) {
					current[z] = up != nullCheck ? filterUpDecode(values->at(index), up[z]) : values->at(index);
				}
				else if (encoded == static_cast<int>(SelectedFilter::Average)) {
					current[z] =left != nullCheck && up != nullCheck ? filterAverageDecode(values->at(index), left[z], up[z]) : values->at(index);
				}
				else if (encoded == static_cast<int>(SelectedFilter::Paeth))
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
