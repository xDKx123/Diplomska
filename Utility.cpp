#include "Utility.h"

/// <summary>
/// Popup v katerem izberemo sliko
/// </summary>
/// <returns>string - ime dokumenta</returns>
std::string Utility::getImage() {
		return "testing.bmp";
}

/// <summary>
/// Prikaz slike
/// </summary>
/// <param name="image">slika, ki jo �elimo prikazati</param>
void Utility::displayImage(cv::Mat image)
{
	cv::imshow("Test", image);
}

/// <summary>
/// Primerjava dveh slik
/// </summary>
/// <param name="image1">slika ena</param>
/// <param name="image2">slika dva</param>
/// <returns>true �e sta sliki enaki, sicer false</returns>
bool Utility::compareImages(cv::Mat image1, cv::Mat image2) {
	if (!image1.empty() and !image2.empty()) {
		for (int x = 0; x < image1.size().height; x++) {
			for (int y = 0; y < image1.size().width; y++) {
				cv::Vec3b v1 = image1.at<cv::Vec3b>(x, y);
				cv::Vec3b v2 = image2.at<cv::Vec3b>(x, y);

				for (int z = 0; z < 3; z++) {
					if (v1[z] != v2[z]) {
						return false;
					}
				}
			}
		}
		return true;
	}
	return false;
}

/// <summary>
/// Prikaz menija
/// </summary>
/// <returns>int - izbira</returns>
int Utility::menu() {
	std::cout << "\n\n----------Meni----------\n" << std::endl;
	std::cout << "1) Naloži dokument" << std::endl;
	std::cout << "2) Poženi kodiranje" << std::endl;
	std::cout << "3) Poženi dekodiranje" << std::endl;
	std::cout << "\n9) Testing" << std::endl;
	//std::cout << "4) Resize image" << std::endl;

	// std::cout << "\nTestiranje" << std::endl;
	// std::cout << "9) Testiranje file comparrison" << std::endl;
	// std::cout << "90) Testiranje filtrov PNG" << std::endl;
	// std::cout << "91) Testiranje transformacije BWT" << std::endl;
	// std::cout << "92) Testiranje transformacije MTF" << std::endl;
	// std::cout << "93) Testiranje algoritma Huffman" << std::endl;

	std::cout << "\n0) Zapri" << std::endl;

	int selection;
	std::cin >> selection;
	return selection;
}

/// <summary>
/// Vrne slovar, ki zajema vseh 256 znakov
/// </summary>
/// <returns>std::vector<char> - 256 znakov</returns>
std::vector<char> Utility::commonDictionaryVector() {
	std::vector<char> v;
	for (int x = 0; x < 256; x++) {
		v.push_back(static_cast<char>(x));
	}
	return v;
}


/// <summary>
/// Vrne slovar, ki zajema vseh 256 znakov
/// </summary>
/// <returns>std::vector<char> - 256 znakov</returns>
std::map<char, int> Utility::commonDictionaryMap() {
	
	std::map<char, int> mp;
	for (int x = 0; x < 256; x++) {
		//v->push_back(static_cast<char>(x));
		mp.insert(std::pair<char, int>( static_cast<char> (x), 0));
	}
	return mp;
}

/// <summary>
/// Funkcija vrne razmeje med originalno sliko, ter stiskano datoteko
/// </summary>
/// <param name="originalFile"></param>
/// <param name="compressedFile"></param>
/// <returns></returns>
double Utility::compressionFactor(std::string originalFile, std::string compressedFile)
{
	try {

		if (originalFile.find(validImageFileExtension) and compressedFile.find(validEncryptedFileExtension)) {
			return static_cast<double>(std::filesystem::file_size(static_cast<std::filesystem::path>(originalFile))) / static_cast<double>(std::filesystem::file_size(static_cast<std::filesystem::path>(compressedFile)));
		}
	}
	catch (...) {

	}
	return 0.0;
}

/// <summary>
/// Zapis podatkov v binarno datoteko
/// </summary>
/// <param name="width">�irina slika</param>
/// <param name="height">vi�ina slike</param>
/// <param name="items">slikovni podatki</param>
/// <param name="encodedValues">binarni podatki posameznega znaka</param>
/// <param name="probability">verjetnostna tabela</param>
void Utility::writeBinFile(int width, int height, int index, std::vector<SelectedFilter> selectedFilter, std::vector<char> items, std::map<char, std::vector<bool>> encodedValues, std::map<char, float> probability, std::string fileName)
{
	//dodaj zapise za glavo
	//std::string fileName = "out" + validEncryptedFileExtension;
	BinWriter* binWriter = new BinWriter(fileName);

	binWriter->writeShort(width);
	binWriter->writeShort(height);

	binWriter->writeInt(index);

	for (int x = 0; x < 256; x++) {
		if (probability.find(static_cast<char>(x)) != probability.end()) {
			binWriter->writeFloat(probability[static_cast<char>(x)]);
		}
		else {
			binWriter->writeFloat(0.0f);
		}
	}

	for (auto currentSelectedFilter : selectedFilter) {
		std::vector<char> selFilter;
		if (currentSelectedFilter == Sub) {
			selFilter.push_back(false);
			selFilter.push_back(false);
		}
		else if (currentSelectedFilter == Up) {
			selFilter.push_back(false);
			selFilter.push_back(true);
		}
		else if (currentSelectedFilter == Average) {
			selFilter.push_back(true);
			selFilter.push_back(false);
		}
		else if (currentSelectedFilter == Paeth) {
			selFilter.push_back(true);
			selFilter.push_back(true);
		}

		for (auto c : selFilter) {
			binWriter->writeBit(c);
		}

		selFilter.clear();
	}




	//for (auto pair : probability) {
	//	//binWriter->writeByte(pair.first);
	//	binWriter->writeFloat(probability[pair]);
	//}

	for (auto item : items) {
		for (auto v : encodedValues[item]) {
			binWriter->writeBit(v);
		}
	}

	for (int x = 0; x < 7; x++) {
		binWriter->writeBit(false);
	}
	binWriter->writeBit(true);

	std::cout << "Ustvarjan dokument: " << fileName << std::endl;

	delete binWriter;
}

/// <summary>
/// Ustvari bmp sliko
/// </summary>
/// <param name="image">slika, ki jo �elimo zapisati na disk</param>
void Utility::writeBmpFile(cv::Mat image, std::string fileName)
{
	//std::string fileName = randomName() + validImageFileExtension;
	//std::string fileName = "out" + validImageFileExtension;

	try {
		cv::imwrite(fileName, image);
		std::cout << "Ustvarjena datoteka: " << fileName << std::endl;
	}
	catch (std::exception& e) {
		std::cout << "Napaka pri zapisu datoteke" << std::endl;
	}
}

/// <summary>
/// Branje binarne datoteke
/// </summary>
/// <returns>�irina, vi�ina, verjetnostna tabela, slikovni podatki</returns>
std::tuple<int, int, int, std::vector<SelectedFilter>, std::vector<bool>, std::map<char, float>> Utility::readBinFile(std::string fileName)
{
	//NE DIRAJ KER DELA
	//std::string fileName = getImage();
	//std::string fileName = "out.bin";
	BinReader* binReader = new BinReader(fileName);

	for (int x = 0; x < 8; x++) {
		binReader->readBit();
	}

	int width = binReader->readShort();
	int height = binReader->readShort();

	int index = binReader->readInt();

	std::map<char, float> probability;
	//int probabilitySize = binReader->readInt();
	//std::cout << "probabilitySize: " << probabilitySize << std::endl;
	for (int x = 0; x < 256; x++) {
		//char c = binReader->readByte();
		float f = binReader->readFloat();
		if (f != 0.0f) {
			probability.insert(std::pair<char, float>(static_cast<char>(x),f));
		}
	}

	std::vector<SelectedFilter> selectedFilter;

	/*if (numberOfEncodedRows == NoneRows) {
		std::vector<bool> selFilter;
		for (int y = 0; y < 2; y++) {
			bool b = binReader->readBit();
			selFilter.push_back(b);
		}

		if (selFilter[0] == false && selFilter[1] == false) {
			selectedFilter.push_back(Sub);
		}
		else if (selFilter[0] == false && selFilter[1] == true) {
			selectedFilter.push_back(Up);
		}
		else if (selFilter[0] == true && selFilter[1] == false) {
			selectedFilter.push_back(Average);
		}
		else if (selFilter[0] == true && selFilter[1] == true) {
			selectedFilter.push_back(Paeth);
		}
	}
	else {*/
		//for (int x = 0; x < static_cast<int>(std::ceil(static_cast<float>(width) / static_cast<int>(numberOfEncodedRows))); x++) {
		for (int x = 0; x < height; x++) {
			std::vector<bool> selFilter;
			for (int y = 0; y < 2; y++) {
				bool b = binReader->readBit();
				selFilter.push_back(b);
			}

			if (selFilter[0] == false && selFilter[1] == false) {
				selectedFilter.push_back(Sub);
			}
			else if (selFilter[0] == false && selFilter[1] == true) {
				selectedFilter.push_back(Up);
			}
			else if (selFilter[0] == true && selFilter[1] == false) {
				selectedFilter.push_back(Average);
			}
			else if (selFilter[0] == true && selFilter[1] == true) {
				selectedFilter.push_back(Paeth);
			}
			selFilter.clear();
		}

	//}
	/*bool b1 = binReader->readBit();
	bool b2 = binReader->readBit();

	if (b1 == false && b2 == false) {
		currentSelectedFilter = Sub;
	}
	else if (b1 == false && b2 == true) {
		currentSelectedFilter = Up;
	}
	else if (b1 == true && b2 == false) {
		currentSelectedFilter = Average;
	}
	else if (b1 == true && b2 == true) {
		currentSelectedFilter = Paeth;
	}*/
	std::cout << "Sel filter:" << selectedFilter.size() << std::endl;

	std::vector<bool> data;
	while (!binReader->isEof()) {
		data.push_back(binReader->readBit());
	}

	while (data.back() != true) {
		data.pop_back();
	}

	for (int x = 0; x < 8; x++) {
		data.pop_back();
	}

	delete binReader;

	return std::tuple<int, int, int, std::vector<SelectedFilter>, std::vector<bool>, std::map<char, float>>(width, height, index, selectedFilter, data, probability);
}

/// <summary>
/// Funkcija, ki spremeni velikost slike
/// </summary>
/// <param name="fileName"></param>
void Utility::resizeImage(std::string fileName)
{
	cv::Mat image(cv::imread(fileName));
	cv::resize(image, image, cv::Size(), 0.1, 0.1);

	cv::imwrite("Test.bmp", image);
}

std::vector<bool> Utility::writeSelectedNumberOfRowsData() {
	std::vector<bool> v;

	/*switch (numberOfEncodedRows) {
		case NoneRows:
			v.push_back(false);
			v.push_back(false);
			v.push_back(false);
			v.push_back(false);
			break;
		case One:
			v.push_back(false);
			v.push_back(false);
			v.push_back(false);
			v.push_back(true);
			break;
		case Two:
			v.push_back(false);
			v.push_back(false);
			v.push_back(true);
			v.push_back(false);
			break;
		case Three:
			v.push_back(false);
			v.push_back(false);
			v.push_back(true);
			v.push_back(true);
			break;
		case Four:
			v.push_back(false);
			v.push_back(true);
			v.push_back(false);
			v.push_back(false);
			break;
		case Five:
			v.push_back(false);
			v.push_back(true);
			v.push_back(false);
			v.push_back(true);
			break;
		case Six:
			v.push_back(false);
			v.push_back(true);
			v.push_back(true);
			v.push_back(false);
			break;
		case Seven:
			v.push_back(false);
			v.push_back(true);
			v.push_back(true);
			v.push_back(true);
			break;
		case Eight:
			v.push_back(true);
			v.push_back(false);
			v.push_back(false);
			v.push_back(false);
			break;
		case Nine:
			v.push_back(true);
			v.push_back(false);
			v.push_back(false);
			v.push_back(true);
			break;
		case Ten:
			v.push_back(true);
			v.push_back(false);
			v.push_back(true);
			v.push_back(false);
			break;
	}*/

	return v;
}


void Utility::writeSelectedNumberOfRowsData(std::vector<bool> b) {
	/*if (b[0] == false && b[1] == false && b[2] == false && b[3] == false) {
		numberOfEncodedRows = NoneRows;
	}
	else if (b[0] == false && b[1] == false && b[2] == false && b[3] == true) {
		numberOfEncodedRows = One;
	}
	else if (b[0] == false && b[1] == false && b[2] == true && b[3] == false) {
		numberOfEncodedRows = Two;
	}
	else if (b[0] == false && b[1] == false && b[2] == true && b[3] == true) {
		numberOfEncodedRows = Three;
	}
	else if (b[0] == false && b[1] == true && b[2] == false && b[3] == false) {
		numberOfEncodedRows = Four;
	}
	else if (b[0] == false && b[1] == true && b[2] == false && b[3] == true) {
		numberOfEncodedRows = Five;
	}
	else if (b[0] == false && b[1] == true && b[2] == true && b[3] == false) {
		numberOfEncodedRows = Six;
	}
		else if (b[0] == false && b[1] == true && b[2] == true && b[3] == true) {
		numberOfEncodedRows = Seven;
	}
		else if (b[0] == true && b[1] == false && b[2] == false && b[3] == false) {
		numberOfEncodedRows = Eight;
	}
		else if (b[0] == true && b[1] == false && b[2] == false && b[3] == true) {
		numberOfEncodedRows = Nine;
	}
		else if (b[0] == true && b[1] == false && b[2] == true && b[3] == false) {
		numberOfEncodedRows = Ten;
	}*/
}

double Utility::SSIM(std::string originalPath, std::string decompressedPath)
{
	const double k1 = 0.01;
	const double k2 = 0.03;

	cv::Mat original = cv::imread(originalPath);
	cv::Mat decompressed =  cv::imread(decompressedPath);


	//cv::Mat originalYCrCb;
	//cv::Mat decompressedYCrCb;
	//cv::cvtColor(original, originalYCrCb, cv::COLOR_RGB2YCrCb);
	//cv::cvtColor(decompressed, decompressedYCrCb, cv::COLOR_RGB2YCrCb);

	

	double qx = averagePixel(original);
	double qy = averagePixel(decompressed);

	double ox = variance(original, qx);
	double oy = variance(decompressed, qy);

	double oxy = covariance(original, qx, decompressed, qy);

	double c1 = dynamicRange(k1);
	double c2 = dynamicRange(k2);

	return ((2*qx*qy + c1)*(2*oxy + c2)) / ((pow(qx,2) + pow(qy,2) + c1)*(pow(ox,2) + pow(oy,2) + c2));
}

double Utility::averagePixel(cv::Mat image) {
	cv::Size imageSize = image.size();

	int counter = 0;
	for (int x = 0; x < imageSize.width; x++) {
		for (int y = 0; y < imageSize.height; y++) {
			cv::Vec3b pixel = image.at<cv::Vec3b>(x, y);

			for (int z = 0; z < 3; z++) {
				counter += pixel[z];
			}
		}
	}

	return static_cast<double>(counter) / (static_cast<double>(imageSize.width * imageSize.height));
}

double Utility::dynamicRange(double k) {
	int L = pow(2, 24) - 1;
	return pow(k * L,2);
}

double Utility::variance(cv::Mat image, double average) {
	cv::Size imageSize = image.size();

	double counter = 0.0;
	for (int x = 0; x < imageSize.width; x++) {
		for (int y = 0; y < imageSize.height; y++) {
			cv::Vec3b pixel = image.at<cv::Vec3b>(x, y);

			for (int z = 0; z < 3; z++) {
				counter += (pixel[z] - average);
			}
		}
	}

	return pow(counter, 0.5);
}

double Utility::covariance(cv::Mat original, double averageOriginal, cv::Mat decompressed, double averageDecompressed) {
	cv::Size imageSize = original.size();

	double counter = 0.0;
	for (int x = 0; x < imageSize.width; x++) {
		for (int y = 0; y < imageSize.height; y++) {
			cv::Vec3b pixelOriginal = original.at<cv::Vec3b>(x, y);
			cv::Vec3b pixelDecompressed = decompressed.at<cv::Vec3b>(x, y);

			for (int z = 0; z < 3; z++) {
				counter += ((pixelOriginal[z] - averageOriginal) * (pixelDecompressed[z] - averageDecompressed));
			}
		}
	}

	return counter;
}