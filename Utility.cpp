#include "Utility.h"

/// <summary>
/// Funkcija zgenerira naklju�no ime
/// </summary>
/// <returns></returns>
std::string Utility::randomName()
{
	//USES_CONVERSION;
	//GUID g;
	//HRESULT hCreateGuid = CoCreateGuid(&g);

	//OLECHAR* guidString;
	//StringFromCLSID(g, &guidString);

	//std::string pString = OLE2CA(static_cast<LPCOLESTR>(guidString));
	////guidStr = pString;
	////guidStr = guidStr.replace(guidStr.begin(), guidStr.end(), '-', '_').substr(1, guidStr.size() - 1);
	//
	//std::replace(pString.begin(), pString.end(), '-', '_');

	//std::string t = pString.substr(1, pString.length() - 2);

	//return pString.substr(1, pString.length() - 2);
	return "";
}

/// <summary>
/// Popup v katerem izberemo sliko
/// </summary>
/// <returns>string - ime dokumenta</returns>
std::string Utility::getImage() {
	//try {

	//	OPENFILENAME ofn;

	//	char szFile[MAXUINT8];

	//	ZeroMemory(&ofn, sizeof(ofn));
	//	ofn.lStructSize = sizeof(ofn);
	//	ofn.hwndOwner = NULL;
	//	ofn.lpstrFile = (LPWSTR)szFile;
	//	ofn.lpstrFile[0] = '\0';
	//	ofn.nMaxFile = sizeof(szFile);
	//	//ofn.lpstrFilter = (LPWSTR)"All\0*.*\0Text\0*.TXT\0";
	//	ofn.nFilterIndex = 1;
	//	ofn.lpstrFileTitle = NULL;
	//	ofn.nMaxFileTitle = 0;
	//	ofn.lpstrInitialDir = NULL;
	//	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	//	if (GetOpenFileName(&ofn)) {
	//		//std::cout << ofn.lpstrFile << std::endl;
	//		return std::string(CW2A(ofn.lpstrFile));
	//	}
	//	else {
	//		//std::cout << "Failed" << std::endl;
	//		return "";
	//	}
	//}
	//catch (const std::exception& ex) {
	//	std::cout << ex.what() << std::endl;
	//	return "";
	//}

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
		//return image1.data == image2.data;
	}
	else {

		return false;
	}
}

/// <summary>
/// Prikaz menija
/// </summary>
/// <returns>int - izbira</returns>
int Utility::menu() {
	std::cout << "\n\nMeni" << std::endl;
	std::cout << "1) Nalo�i dokument" << std::endl;
	std::cout << "2) Po�eni kodiranje" << std::endl;
	std::cout << "3) Po�eni dekodiranje" << std::endl;
	//std::cout << "4) Resize image" << std::endl;

	std::cout << "\nTestiranje" << std::endl;
	std::cout << "9) Testiranje file comparrison" << std::endl;
	std::cout << "90) Testiranje filtrov PNG" << std::endl;
	std::cout << "91) Testiranje transformacije BWT" << std::endl;
	std::cout << "92) Testiranje transformacije MTF" << std::endl;
	std::cout << "93) Testiranje algoritma Huffman" << std::endl;

	std::cout << "\n0) Zapri" << std::endl;

	int selection;
	std::cin >> selection;
	return selection;
}

/// <summary>
/// Vrne slovar, ki zajema vseh 256 znakov
/// </summary>
/// <returns>std::vector<char> - 256 znakov</returns>
std::vector<char>* Utility::commonDictionaryVector() {
	std::vector<char>* v = new std::vector<char>;
	for (int x = 0; x < 256; x++) {
		v->push_back(static_cast<char>(x));
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
void Utility::writeBinFile(int width, int height, int index, std::vector<char>* items, std::map<char, std::vector<bool>> encodedValues, std::map<char, float> probability)
{
	//dodaj zapise za glavo
	//std::string fileName = randomName() + validEncryptedFileExtension;
	std::string fileName = "out" + validEncryptedFileExtension;
	BinWriter* binWriter = new BinWriter(fileName);

	binWriter->writeInt(width);
	binWriter->writeInt(height);

	binWriter->writeInt(index);

	binWriter->writeInt(probability.size());
	for (auto pair : probability) {
		binWriter->writeByte(pair.first);
		binWriter->writeFloat(pair.second);
	}

	for (auto item : *items) {
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
void Utility::writeBmpFile(cv::Mat image)
{
	//std::string fileName = randomName() + validImageFileExtension;
	std::string fileName = "out" + validImageFileExtension;

	if (cv::imwrite(fileName, image)) {
		std::cout << "Ustvarjena datoteka: " << fileName << std::endl;
	}
	else {
		std::cout << "Napaka pri zapisu datoteke" << std::endl;
	}
}

/// <summary>
/// Branje binarne datoteke
/// </summary>
/// <returns>�irina, vi�ina, verjetnostna tabela, slikovni podatki</returns>
std::tuple<int, int,int, std::vector<bool>*, std::map<char, float>> Utility::readBinFile()
{
	//NE DIRAJ KER DELA
	//std::string fileName = getImage();
	std::string fileName = "out.bin";
	BinReader* binReader = new BinReader(fileName);

	for (int x = 0; x < 8; x++) {
		binReader->readBit();
	}

	int width = binReader->readInt();
	int height = binReader->readInt();

	int index = binReader->readInt();

	std::map<char, float> probability;
	int probabilitySize = binReader->readInt();
	std::cout << "probabilitySize: " << probabilitySize << std::endl;
	for (int x = 0; x < probabilitySize; x++) {
		char c = binReader->readByte();
		float f = binReader->readFloat();
		probability.insert(std::pair<char, float>(c,f));
	}

	std::vector<bool>* data = new std::vector<bool>;
	while (!binReader->isEof()) {
		data->push_back(binReader->readBit());
	}

	while (data->back() != true) {
		data->pop_back();
	}

	for (int x = 0; x < 8; x++) {
		data->pop_back();
	}

	delete binReader;

	return std::tuple<int, int, int, std::vector<bool>*, std::map<char, float>>(width, height, index, data, probability);
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
