#include <iostream>
#include <string.h>
#include <Windows.h>
#include <atlstr.h>
#include <locale>
#include <chrono>
#include "PNG_filters.h"
#include "Utility.h"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char* argv) {
	setlocale(LC_ALL, "slovenian");
	bool running = true;
	PNG_filters *pngFilters = NULL;

	//pngFilters->showImage();

	while (running) {
		switch (Utility::menu()) {
		case 1:
			pngFilters = new PNG_filters(Utility::getImage());
			break;

		case 2:
			if (pngFilters) {
				std::chrono::time_point start = std::chrono::system_clock::now();
				std::vector<PNG_filters::Values*>* v = pngFilters->algorithm();
				std::chrono::time_point end = std::chrono::system_clock::now();
				std::cout << "Trajanje filtriranja: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
			}
			else {
				std::cout << "Najprej naložite sliko." << std::endl;
			}
			break;

		case 0:
			running = false;
			break;
		default:
			std::cout << "Napaèna izbira." << std::endl;
			break;
		}


	}

	delete pngFilters;
	return 0;
}