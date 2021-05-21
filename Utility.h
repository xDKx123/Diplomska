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
#include <random>
#include <uuids.h>

#include "BinReader.h"
#include "BinWriter.h"
#include "PNG_filters.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>


class Utility
{
private:
	static std::string randomName();
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
	static void writeBinFile(int width, int height, std::vector<char>* items, std::map<char, std::vector<bool>> encodedValues, std::map<char, float> probability);
	static void writeBmpFile(cv::Mat image);
	static std::tuple<int, int, std::vector<bool>*, std::map<char, float>> readBinFile();
	//MessageBox(NULL, ofn.lpstrFile, (LPCWSTR)"File Name", MB_OK););

	static void resizeImage(std::string fileName);
};

