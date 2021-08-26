#pragma once
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <optional>

#include "Utility.h"

class Huffman
{
private:
	struct Node {
		std::pair<std::optional<unsigned char>, float> p;
		Node* left, *right;

		Node(std::pair<std::optional<unsigned char>, float> pa) {
			p = pa;
			left = NULL;
			right = NULL;
		}
	};

	struct compare {
		bool operator()(Node* l, Node* r) {
			return l->p.second > r->p.second;
		};
	};

	void makeCodes(std::map<unsigned char, std::vector<bool>>& v, struct Node* root, std::vector<bool> b);

	//std::vector<std::pair<char, int>> sortedVectorOfValues(std::map<char, int> mp);
	std::map<unsigned char, std::vector<bool>> buildTree(std::map<unsigned char, float> v);
	//std::map<char, std::vector<bool>> buildTreeDecompression(std::map<char, float> v);
	std::map<unsigned char, float> calculateProbability(std::map<unsigned char, int> mp, int size);

public:
	std::tuple<std::map<unsigned char, std::vector<bool>>, std::map<unsigned char, float>> Encode(std::vector<unsigned char> v);
	std::vector<unsigned char> Decode(std::vector<bool> data, std::map<unsigned char, float> cf);
};

