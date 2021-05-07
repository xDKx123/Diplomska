#include "Utility.h"

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

bool Utility::compareImages(cv::Mat image1, cv::Mat image2) {
	if (!image1.empty() and !image2.empty()) {
		return image1.data == image2.data;
	}
}

int Utility::menu() {
	std::cout << "Meni" << std::endl;
	std::cout << "1) Naloži sliko" << std::endl;
	std::cout << "2) Poženi algoritem" << std::endl;

	std::cout << "\n0) Zapri" << std::endl;

	int selection;
	std::cin >> selection;
	return selection;
}