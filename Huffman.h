#pragma once
#include <vector>
#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <optional>
class Huffman
{
private:
	static struct Node {
		std::pair<std::optional<char>, int> p;
		Node* left, *right;

		Node(std::pair<std::optional<char>, int> pa) {
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

	void makeCodes(std::map<char, std::vector<bool>>& v, struct Node* root, std::vector<bool> b);
	std::vector<std::pair<char, int>> sortedVectorOfValues(std::map<char, int> mp);
	std::map<char, std::vector<bool>> buildTree(std::vector<std::pair<char, int>> v);

public:
	std::vector<bool>* Encode(std::vector<char>* v);
	std::vector<char>* Decode(std::vector<bool>* v);


};

