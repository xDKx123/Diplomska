#pragma once
#include <string>
//#include <Windows.h>
//#include <atlstr.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <list>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>
#include <random>
//#include <uuids.h>

#include "BinReader.h"
#include "BinWriter.h"
#include "PNG_filters.h"
#include "SelectedFilter.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>


class Utility
{
private:
	static double averagePixel(cv::Mat image);
	static double dynamicRange(double k);
	static double variance(cv::Mat image, double average);
	static double covariance(cv::Mat original, double averageOriginal, cv::Mat decompressed, double averageDecompressed);
public:
	inline static const std::string validImageFileExtension = ".bmp";
	inline static const std::string validEncryptedFileExtension = ".bin";


	static std::string getImage();
	static void displayImage(cv::Mat image);
	static bool compareImages(cv::Mat image1, cv::Mat image2);
	static int menu();
	static std::vector<unsigned char> commonDictionaryVector();
	static std::map<unsigned char, int> commonDictionaryMap();

	static double compressionFactor(std::string originalFile, std::string compressedFile);
	static void writeBinFile(int width, int height,int index, std::vector<SelectedFilter> selectedFilter, std::vector<unsigned char> items, std::map<unsigned char, std::vector<bool>> encodedValues, std::map<unsigned char, float> probability, std::string fileName);
	static void writeBmpFile(cv::Mat image, std::string fileName);
	static std::tuple<int, int, int, std::vector<SelectedFilter>, std::vector<bool>, std::map<unsigned char, float>> readBinFile(std::string fileName);

	static void resizeImage(std::string fileName);

	static std::vector<bool> writeSelectedNumberOfRowsData();
	static void writeSelectedNumberOfRowsData(std::vector<bool> b);

	static double SSIM(std::string originalPath, std::string decompressedPath);
};

