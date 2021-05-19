#include "Utility.h"

/// <summary>
/// Popup v katerem izberemo sliko
/// </summary>
/// <returns>string - ime dokumenta</returns>
std::string Utility::getImage() {
	try {

		OPENFILENAME ofn;

		char szFile[MAXUINT8];

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFile = (LPWSTR)szFile;
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		//ofn.lpstrFilter = (LPWSTR)"All\0*.*\0Text\0*.TXT\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		if (GetOpenFileName(&ofn)) {
			//std::cout << ofn.lpstrFile << std::endl;
			return std::string(CW2A(ofn.lpstrFile));
		}
		else {
			//std::cout << "Failed" << std::endl;
			return "";
		}
	}
	catch (const std::exception& ex) {
		std::cout << ex.what() << std::endl;
		return "";
	}
}

void Utility::displayImage(cv::Mat image)
{
	cv::imshow("Test", image);
}

/// <summary>
/// Primerjava dveh slik
/// </summary>
/// <param name="image1">slika ena</param>
/// <param name="image2">slika dva</param>
/// <returns>true èe sta sliki enaki, sicer false</returns>
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
	std::cout << "1) Naloži dokument" << std::endl;
	std::cout << "2) Poženi kodiranje" << std::endl;
	std::cout << "3) Poženi dekodiranje" << std::endl;
	std::cout << "\n4) Testiranje" << std::endl;
	std::cout << "\n9) Testiranje file comparrison" << std::endl;
	std::cout << "\n90) Testiranje filtrov" << std::endl;

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

void Utility::writeToFile(int width, int height, std::map<char, std::vector<bool>> mp)
{
	//dodaj zapise za glavo

	for (auto x : mp) {
		for (auto b : x.second) {
			//klici write za bool
		}
	}

	//Dodaj da se na koncu dadjo še simboli, da ves kje je eof

	for (int x = 0; x < 7; x++) {
		//write false
	}
	//write true;
}
