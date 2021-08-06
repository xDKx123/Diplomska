#include <iostream>
#include <string.h>
//#include <Windows.h>
//#include <atlstr.h>
#include <locale>
#include <chrono>
#include <list>

#include "PNG_filters.h"
#include "Utility.h"
#include "MTF.h"
#include "Huffman.h"
#include "Huffman.cpp"

#include "additional_library/bwt.hpp"

//#include <opencv4/opencv2/imgcodecs.hpp>
//#include <opencv4/opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char* argv) {
	setlocale(LC_ALL, "slovenian");
	bool running = true;
	PNG_filters *pngFilters = NULL;

	//pngFilters->showImage();

	std::string fileName = "testing.bmp";

	while (running) {
		switch (Utility::menu()) {
		case 1: {
			//pngFilters = new PNG_filters(Utility::getImage());
			fileName = Utility::getImage();
			pngFilters = new PNG_filters(fileName);
		}
			break;

		case 2: {
			numberOfEncodedRows = NoneRows;

			if (pngFilters) {
				auto start = std::chrono::system_clock::now();
				std::vector<char>* v = pngFilters->Encode();
				cv::Size size = pngFilters->getSize();
				auto end = std::chrono::system_clock::now();
				std::cout << "Trajanje filtriranja: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;


				start = std::chrono::system_clock::now();
				auto key = townsend::algorithm::bwtEncode(v->begin(), v->end());
				end = std::chrono::system_clock::now();
				int index = std::distance(v->begin(), key);
				std::cout << "Trajanje BWT: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

				MTF* mtf = new MTF();
				start = std::chrono::system_clock::now();
				std::vector<char>* mtfTransformed = mtf->Encode(v);
				end = std::chrono::system_clock::now();
				std::cout << "Trajanje MTF: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;



				Huffman<float>* hf = new Huffman<float>();

				start = std::chrono::system_clock::now();
				std::map<char, std::vector<bool>> tree;
				std::map<char, float> probability;
				std::tie(tree, probability) = hf->Encode(mtfTransformed);
				end = std::chrono::system_clock::now();
				std::cout << "Trajanje Huffman: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;


				// std::cout << "--------------DEBUG INFO--------------" << std::endl;
				// for (auto it : probability)  {
				// 	std::cout << it.first << "\t" << it.second << std::endl; 
				// }
				// std::cout << "--------------DEBUG INFO--------------" << std::endl;


				start = std::chrono::system_clock::now();
				Utility::writeBinFile(size.width, size.height, index, mtfTransformed, tree, probability);
				end = std::chrono::system_clock::now();
				std::cout << "Trajanje zapisovanja v bin datoteko: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
				v->clear();


				std::cout << "\nFile compression: " << Utility::compressionFactor(fileName, "out.bin") << std::endl;
				

				delete v;
				delete mtf;
				delete hf;
			}
			else {
				std::cout << "Najprej nalo�ite sliko." << std::endl;
			}
		}
			break;


		case 3: {
			int width, height, index;
			std::vector<bool>* data;
			std::map<char, float> probability;

			auto start = std::chrono::system_clock::now();
			std::tie(width, height, index, data, probability) = Utility::readBinFile();
			auto end = std::chrono::system_clock::now();
			std::cout << "Trajanje branja binarne datoteke: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;


			//std::cout << "data: " << data->size() << std::endl;

			Huffman<float>* huffman = new Huffman<float>();

			start = std::chrono::system_clock::now();
			std::vector<char>* chars = huffman->Decode(data, probability);
			end = std::chrono::system_clock::now();
			std::cout << "Trajanje dekodiranja Huffman: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;


			//std::cout << "data: " << chars->size() << std::endl;

			MTF* mtf = new MTF();
			start = std::chrono::system_clock::now();
			std::vector<char>* mtfDecode = mtf->Decode(chars);
			end = std::chrono::system_clock::now();
			std::cout << "Trajanje iMTF: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;


			auto key2 = std::next(mtfDecode->begin(), index);
			start = std::chrono::system_clock::now();
			townsend::algorithm::bwtDecode(mtfDecode->begin(), mtfDecode->end(), key2);
			end = std::chrono::system_clock::now();
			std::cout << "Trajanje iBWT: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

			//std::cout << width << " " << height << " " << mtfDecode->size() << std::endl;

			PNG_filters* png = new PNG_filters();

			start = std::chrono::system_clock::now();
			cv::Mat image = png->Decode(width, height, mtfDecode);
			end = std::chrono::system_clock::now();
			std::cout << "Trajanje PNG dekodiranja: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
			Utility::writeBmpFile(image);

		}
			  break;

		// case 4: {
		// 	std::string file = Utility::getImage();

		// 	Utility::resizeImage(file);
		// }
		// 	  break;

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
			std::cout << "TEST:" << std::endl;

			std::vector<char> testingBWT;
			std::vector<char> check;
			
			for (int x = 0; x < 100; x++) {
				for (int y = 0; y < 256;y++) {
					char ca = rand() % 256;
					testingBWT.push_back(static_cast<char>(ca));
					check.push_back(static_cast<char>(ca));
				}
			}

			std::cout << "Generated array" << std::endl;

			auto key = townsend::algorithm::bwtEncode(testingBWT.begin(), testingBWT.end());

			// for (int x = 0; x < 100; x++) {
			// 	std::cout << testingBWT[x];
			// }
			// std::cout << std::endl;

			int index = std::distance(testingBWT.begin(),key);

			std::cout << "Encode" << std::endl;


			BinWriter *bw = new BinWriter("testingBwt.bin");
			bw->writeInt(index);
			// std::cout << "index: " << testingBWT.size() << std::endl;
			for (auto it : testingBWT) {
				bw->writeByte(it);
			// 	std::cout << it;
			 }
			// std::cout << std::endl;

			// for (int x = 0; x < 7; x++) {
			// 	bw->writeBit(false);
			// }
			// bw->writeBit(true);

			delete bw;

			std::vector<char> data;

			BinReader *br = new BinReader("testingBwt.bin");
			for (int x = 0; x < 8; x++) {
				bool b = br->readBit();
			}

			index = br->readInt();

			while (!br->isEof()) {
				data.push_back(br->readByte());
			}

			data.pop_back();

			for (int x = 0; x < 100; x++) {
				std::cout << data[x];
			}
			std::cout << std::endl;
			// while (data.back() != true) {
			// 	data.pop_back();
			// }

			// for (int x = 0; x < 8; x++) {
			// 	data.pop_back();
			// }

			delete br;

			// std::cout << "index: " << data.size() << std::endl;
			
			auto key2 = std::next(data.begin(), index);

			townsend::algorithm::bwtDecode(data.begin(), data.end(), key2);

			std::cout << "Decode" << std::endl;

			// for (auto it : testingBWT) {
			// 	std::cout << it;
			// }
			// std::cout << std::endl;

			// for (int x = 0; x < testingBWT.size(); x++) {
			// 	if (data[x] != check[x]) {
			// 		std::cout << "Test failed at index: " << x << std::endl;
			// 		break;
			// 	}
			// }

			std::cout << "Test passed" << std::endl;
			
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

			delete mtf;

			mtf = new MTF();

			std::vector<char>* decodec = mtf->Decode(res);

			std::cout << std::endl;
			
		}
			   break;
		case 93: {
			//Huffman
			std::vector<char>* v = new std::vector<char>;
			v->push_back('t');
			v->push_back('e');
			v->push_back('s');
			v->push_back('t');
			v->push_back('1');
			v->push_back('2');
			v->push_back('3');
			v->push_back('a');
			v->push_back('b');
			v->push_back('c');
			v->push_back('d');
			v->push_back('e');
			v->push_back('f');
			v->push_back('\n');
			v->push_back('\n');
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

			MTF* mtf1 = new MTF();
			std::vector<char>* mtf1Dec = mtf1->Encode(v);

			Huffman<int>* hff = new Huffman<int>();
			
			std::map<char, std::vector<bool>> tree;
			std::map<char, float> probability;
			std::tie(tree, probability) = hff->Encode(mtf1Dec);

			delete hff;

			Utility::writeBinFile(0, 0,0, mtf1Dec, tree, probability);

			std::cout << "mtf: " << mtf1Dec->size() << std::endl;

			int width, height, idx;
			std::vector<bool>* data;
			std::map<char, float> probability2;

			std::tie(width, height, idx, data, probability2) = Utility::readBinFile();

			Huffman<float>* huffman = new Huffman<float>();
			std::vector<char>* chars = huffman->Decode(data, probability);

			MTF* mtf = new MTF();
			std::vector<char>* mtfDec = mtf->Decode(chars);

			std::cout << "mtf: " << mtfDec->size() << std::endl;
			//std::cout << "NOT YET IMPLEMENTED" << std::endl;
		}
			   break;
		case 0:
			running = false;
			break;

		default:
			std::cout << "Napa�na izbira." << std::endl;
			break;
		}
	}

	delete pngFilters;
	return 0;
}