#include "Huffman.h"
#include "Utility.h"


void Huffman::makeCodes(std::map<char, std::vector<bool>>& v, struct Node* root, std::vector<bool> b)
{
	if (!root) {
		return;
	}

	if (root->p.first) {
		//v.push_back(std::make_pair<char, int>(root->p.first, root->p.second));
		v.insert(std::pair<char, std::vector<bool >>(*root->p.first, b));
	}
	std::vector<bool> b0 = b;
	std::vector<bool> b1 = b;

	b0.push_back(false);
	b1.push_back(true);

	makeCodes(v, root->left, b0);
	makeCodes(v, root->right, b1);
}

//std::vector<std::pair<char, int>> Huffman::sortedVectorOfValues(std::map<char, int> mp) {
//	std::vector<std::pair<char, int>> vec;
//	for (std::map<char, int>::iterator it = mp.begin(); it != mp.end(); ++it) {
//		if (it->second != 0) {
//			vec.push_back(std::pair<char, int>(it->first, it->second));
//		}
//	}
//
//	std::sort(vec.begin(), vec.end(), [](const std::pair<char, int>& a, const std::pair<char, int>& b) {return a.second < b.second; });
//
//	return vec;
//}

std::map<char, std::vector<bool>> Huffman::buildTree(std::map<char, int> v)
{
	std::map<char, std::vector<bool>> mp;

	std::priority_queue<Node*, std::vector<Node*>, compare> prior_q;
	
	Node* l, * r, *top;
	for (auto &pair : v) {
		if (pair.second != 0) {
			prior_q.push(new Node(pair));
		}
	}

	while (prior_q.size() != 1) {
		l = prior_q.top();
		prior_q.pop();

		r = prior_q.top();
		prior_q.pop();

		top = new Node(std::make_pair<std::optional<char>, int>({}, l->p.second + r->p.second));

		top->left = l;
		top->right = r;


		prior_q.push(top);
	}
	
	std::vector<bool> b;
	makeCodes(mp, prior_q.top(), b);

	return mp;
}


std::vector<bool>* Huffman::Encode(std::vector<char>* v) {
	//class T::value_type s;
	//std::cout << s << std::endl;
	

	std::vector<bool>* b = new std::vector<bool>;

	
	//std::cout << typeid(v).name() << std::endl;

	//if (typeid(v) == typeid(std::vector<char>*)) {

	//}

	std::map<char, int> mp = Utility::commonDictionaryMap();

	for (auto c : *v) {
		//std::map<char, int>::iterator it = std::find(mp.begin(), mp.end(), c);
		mp[c]++;
	}

	//mp.erase(std::remove_if(mp.begin(), mp.end(), [](std::pair<char, int> p) { return p.second != 0; }), mp.end());

	//std::vector<std::pair<char, int>> vec = sortedVectorOfValues(mp);

	buildTree(mp);

	return b;
}


std::vector<char>* Huffman::Decode(std::vector<bool>* v) {
	std::vector<char>* b = new std::vector<char>;

	return b;


}

