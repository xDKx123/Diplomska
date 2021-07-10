#!/bin/bash
FLAGS="$(pkg-config --cflags --libs opencv4)"
g++ -w -O3 -std=c++17 BinReader.cpp BinWriter.cpp Huffman.cpp MTF.cpp PNG_filters.cpp Utility.cpp main.cpp $FLAGS && ./a.out
