#pragma once
#include <string>
#include <Windows.h>
#include <atlstr.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

class Utility
{
public:
	inline static const std::string validImageFileExtension = ".bmp";
	inline static const std::string validEncryptedFileExtension = ".bin";


	static std::string getImage();
	static void displayImage(cv::Mat image);
	static bool compareImages(cv::Mat image1, cv::Mat image2);
	static int menu();
	static std::vector<char>* commonDictionaryVector();
	static std::map<char, int> commonDictionaryMap();

	static double compressionFactor(std::string originalFile, std::string compressedFile);
	static void writeToFile(int width, int height, std::map<char, std::vector<bool>> mp);
	//MessageBox(NULL, ofn.lpstrFile, (LPCWSTR)"File Name", MB_OK););
};

