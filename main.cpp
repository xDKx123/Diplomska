#include <iostream>
#include <string.h>
#include <Windows.h>
#include <atlstr.h>
#include <locale>
#include <chrono>
#include <list>

#include "PNG_filters.h"
#include "Utility.h"
#include "MTF.h"
#include "bwt.hpp"
#include "Huffman.h"
#include "Huffman.cpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char* argv) {
	setlocale(LC_ALL, "slovenian");
	bool running = true;
	PNG_filters *pngFilters = NULL;

	//pngFilters->showImage();

	while (running) {
		switch (Utility::menu()) {
		case 1: {
			pngFilters = new PNG_filters(Utility::getImage());
		}
			break;

		case 2: {
			if (pngFilters) {
				auto start = std::chrono::system_clock::now();
				std::vector<char>* v = pngFilters->Encode();
				cv::Size size = pngFilters->getSize();
				auto end = std::chrono::system_clock::now();
				std::cout << "Trajanje filtriranja: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

				MTF* mtf = new MTF();
				start = std::chrono::system_clock::now();
				std::vector<char>* mtfTransformed = mtf->Encode(v);
				end = std::chrono::system_clock::now();
				std::cout << "Trajanje mtf: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

				Huffman<int>* hf = new Huffman<int>();

				start = std::chrono::system_clock::now();
				std::map<char, std::vector<bool>> tree;
				std::map<char, float> probability;
				std::tie(tree, probability) = hf->Encode(mtfTransformed);
				end = std::chrono::system_clock::now();
				std::cout << "Trajanje huffman: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

				start = std::chrono::system_clock::now();
				Utility::writeBinFile(size.width, size.height, mtfTransformed, tree, probability);
				end = std::chrono::system_clock::now();
				std::cout << "Trajanje zapisovanja v bin datoteko: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
				v->clear();
				delete v;
				delete mtf;
				delete hf;
			}
			else {
				std::cout << "Najprej naložite sliko." << std::endl;
			}
		}
			break;


		case 3: {
			int width, height;
			std::vector<bool>* data;
			std::map<char, float> probability;

			std::tie(width, height, data, probability) = Utility::readBinFile();

			Huffman<float>* huffman = new Huffman<float>();
			std::vector<char>* chars = huffman->Decode(width, height, data, probability);

		}
			  break;
		case 4: {
			/*std::vector<char>* v = new std::vector<char>;
			v->push_back('p');
			v->push_back('a');
			v->push_back('n');
			v->push_back('a');
			v->push_back('m');
			v->push_back('a');


			std::vector<char>* dict = new std::vector<char>;
			dict->push_back('a');
			dict->push_back('b');
			dict->push_back('c');
			dict->push_back('d');
			dict->push_back('e');
			dict->push_back('f');
			dict->push_back('g');
			dict->push_back('h');
			dict->push_back('i');
			dict->push_back('j');
			dict->push_back('k');
			dict->push_back('l');
			dict->push_back('m');
			dict->push_back('n');
			dict->push_back('o');
			dict->push_back('p');
			dict->push_back('q');
			dict->push_back('r');
			dict->push_back('s');
			dict->push_back('t');
			dict->push_back('u');
			dict->push_back('v');
			dict->push_back('w');
			dict->push_back('x');
			dict->push_back('y');
			dict->push_back('z');
			MTF* mtf = new MTF(dict);

			std::vector<int>* enc = mtf->Encode(v);



			dict->clear();
			dict->push_back('a');
			dict->push_back('b');
			dict->push_back('c');
			dict->push_back('d');
			dict->push_back('e');
			dict->push_back('f');
			dict->push_back('g');
			dict->push_back('h');
			dict->push_back('i');
			dict->push_back('j');
			dict->push_back('k');
			dict->push_back('l');
			dict->push_back('m');
			dict->push_back('n');
			dict->push_back('o');
			dict->push_back('p');
			dict->push_back('q');
			dict->push_back('r');
			dict->push_back('s');
			dict->push_back('t');
			dict->push_back('u');
			dict->push_back('v');
			dict->push_back('w');
			dict->push_back('x');
			dict->push_back('y');
			dict->push_back('z');
			mtf = new MTF(dict);

			mtf->Decode(enc);

			*/


			//std::vector<char>m;
			//m.push_back('p');
			//m.push_back('a');
			//m.push_back('n');
			//m.push_back('a');
			//m.push_back('m');
			//m.push_back('a');



			std::vector<char>* v = new std::vector<char>;
			v->push_back('a');
			v->push_back('a');
			v->push_back('a');
			v->push_back('a');
			v->push_back('a');

			v->push_back('b');
			v->push_back('b');
			v->push_back('b');
			v->push_back('b');
			v->push_back('b');
			v->push_back('b');
			v->push_back('b');
			v->push_back('b');
			v->push_back('b');

			v->push_back('c');
			v->push_back('c');
			v->push_back('c');
			v->push_back('c');
			v->push_back('c');
			v->push_back('c');
			v->push_back('c');
			v->push_back('c');
			v->push_back('c');
			v->push_back('c');
			v->push_back('c');
			v->push_back('c');

			v->push_back('d');
			v->push_back('d');
			v->push_back('d');
			v->push_back('d');
			v->push_back('d');
			v->push_back('d');
			v->push_back('d');
			v->push_back('d');
			v->push_back('d');
			v->push_back('d');
			v->push_back('d');
			v->push_back('d');
			v->push_back('d');

			v->push_back('e');
			v->push_back('e');
			v->push_back('e');
			v->push_back('e');
			v->push_back('e');
			v->push_back('e');
			v->push_back('e');
			v->push_back('e');
			v->push_back('e');
			v->push_back('e');
			v->push_back('e');
			v->push_back('e');
			v->push_back('e');
			v->push_back('e');
			v->push_back('e');
			v->push_back('e');


			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');
			v->push_back('f');



			Huffman<int>* huf = new Huffman<int>();
			huf->Encode(v);

			//Huffman::Encode(v);
			
			delete v;
			}
			break;

		case 9: {
			std::string file1 = Utility::getImage();
			std::string file2 = Utility::getImage();

			std::cout << "\nFile compression: " << Utility::compressionFactor(file1, file2) << std::endl;

		}
			  break;

		case 90: {
			std::string file1 = Utility::getImage();

			PNG_filters* png1 = new PNG_filters(file1);
			cv::Size sz = png1->getSize();
			cv::Mat mat1 = png1->getImage();

			std::vector<char> *res = png1->Encode();

			png1 = new PNG_filters(file1);
			cv::Mat mat2 = png1->Decode(sz.width, sz.height, res);

			std::cout << "Image is same: " << Utility::compareImages(mat1, mat2) <<std::endl;
		}
			   break;

		case 91: {
			//BWT
			std::cout << "NOT YET IMPLEMENTED" << std::endl;
		}
			   break;

		case 92: {
			//MTF
			std::vector<char>* v = new std::vector<char>;
			v->push_back('p');
			v->push_back('a');
			v->push_back('n');
			v->push_back('a');
			v->push_back('m');
			v->push_back('a');

			MTF* mtf = new MTF();
			std::vector<char>* res = mtf->Encode(v);

			std::vector<char>* decodec = mtf->Encode(res);
			
		}
			   break;
		case 93: {
			//Huffman
			std::vector<char>* v = new std::vector<char>;
			v->push_back('d');
			v->push_back('a');
			v->push_back('v');
			v->push_back('i');
			v->push_back('d');
			v->push_back('u');
			v->push_back('r');
			v->push_back('s');
			v->push_back('k');
			v->push_back('a');
			v->push_back('i');
			v->push_back('v');
			v->push_back('a');
			v->push_back('n');
			v->push_back('m');
			v->push_back('a');
			v->push_back('t');
			v->push_back('e');
			v->push_back('j');
			v->push_back('k');
			v->push_back('u');
			v->push_back('j');
			v->push_back('n');
			v->push_back('e');
			v->push_back('k');
			v->push_back('e');

			Huffman<int>* hff = new Huffman<int>();
			
			std::map<char, std::vector<bool>> tree;
			std::map<char, float> probability;
			std::tie(tree, probability) = hff->Encode(v);

			delete hff;

			Utility::writeBinFile(0, 0, v, tree, probability);

			int width, height;
			std::vector<bool>* data;
			std::map<char, float> probability2;

			std::tie(width, height, data, probability2) = Utility::readBinFile();

			Huffman<float>* huffman = new Huffman<float>();
			std::vector<char>* chars = huffman->Decode(width, height, data, probability);


			//std::cout << "NOT YET IMPLEMENTED" << std::endl;
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