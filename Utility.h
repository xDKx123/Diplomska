#pragma once
#include <string>
#include <Windows.h>
#include <atlstr.h>
#include <exception>
#include <iostream>
#include <sstream>
#include <list>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

class Utility
{
public:
	static std::string getImage();
	static bool compareImages(cv::Mat image1, cv::Mat image2);
	static int menu();
	static std::vector<char>* commonDictionary();
	//MessageBox(NULL, ofn.lpstrFile, (LPCWSTR)"File Name", MB_OK););
};

