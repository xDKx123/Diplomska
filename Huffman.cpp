#include "Huffman.h"

/// <summary>
/// Za vsak znak dobimo njegov binarni zapis
/// </summary>
/// <typeparam name="T">float - verjetnostna tabela, int - �tevilo pojavitev</typeparam>
/// <param name="v">znaki z njihovimi podatki</param>
/// <param name="root">koren</param>
/// <param name="b">binarni vektor</param>
void Huffman::makeCodes(std::map<unsigned char, std::vector<bool>>& v, struct Node* root, std::vector<bool> b)
{
	if (!root) {
		return;
	}

	if (root->p.first) {
		//v.push_back(std::make_pair<char, int>(root->p.first, root->p.second));
		v.insert(std::pair<unsigned char, std::vector<bool >>(*root->p.first, b));
	}
	std::vector<bool> b0 = b;
	std::vector<bool> b1 = b;

	b0.push_back(false);
	b1.push_back(true);

	makeCodes(v, root->left, b0);
	makeCodes(v, root->right, b1);
}

/// <summary>
/// Zgradimo Huffmanovo dravo
/// </summary>
/// <typeparam name="T">float - verjetnostna tabela, int - �tevilo pojavitev</typeparam>
/// <param name="v">verjetnostna tabela / tabela, ki �teje pojavitve</param>
/// <returns></returns>
std::map<unsigned char, std::vector<bool>> Huffman::buildTree(std::map<unsigned char, float> v)
{
	std::map<unsigned char, std::vector<bool>> mp;

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

		top = new Node(std::make_pair<std::optional<unsigned char>, float>({}, l->p.second + r->p.second));

		top->left = l;
		top->right = r;


		prior_q.push(top);
	}
	
	std::vector<bool> b;
	makeCodes(mp, prior_q.top(), b);

	return mp;
}

/// <summary>
/// Za vsak znak zra�una verjetnost pojavitve
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="mp">pojavitvena tabela</param>
/// <param name="tree">zgrajeno drevo</param>
/// <returns></returns>
std::map<unsigned char, float> Huffman::calculateProbability(std::map<unsigned char, int> mp, int size)
{
	std::map<unsigned char, float> cf;

	for (auto &p : mp) {
		if (p.second != 0) {
			cf.insert(std::pair<unsigned char, float>(p.first, static_cast<float>(static_cast<double>(p.second) / size)));
		}
	}
	//for (auto p : tree) {
	//	cf.insert(std::pair<char, float>(p.first, static_cast<float>(mp[p.first]) / tree.size()));
	//}

	return cf;
}

/// <summary>
/// Huffmanovo kodiranje
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="v">slikovni podatki</param>
/// <returns>tabela z binarnimi podatki znakov, verjetnostna tabela</returns>
std::tuple<std::map<unsigned char, std::vector<bool>>, std::map<unsigned char, float>> Huffman::Encode(std::vector<unsigned char> v)
{
	//std::vector<bool> b;

	std::map<unsigned char, int> mp = Utility::commonDictionaryMap();

	for (auto c : v) {
		//std::map<char, int>::iterator it = std::find(mp.begin(), mp.end(), c);
		mp[c]++;
	}

	std::map<unsigned char, float> cf = calculateProbability(mp, v.size());

	std::map<unsigned char, std::vector<bool>> tree = buildTree(cf);

	return std::tuple<std::map<unsigned char, std::vector<bool>>, std::map<unsigned char, float>>(tree, cf);
}

/// <summary>
/// Huffmanovo dekodiranje
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="data">binarni podatki</param>
/// <param name="cf">verjetnostna tabela</param>
/// <returns>slikovni podatki</returns>
std::vector<unsigned char> Huffman::Decode(std::vector<bool> data, std::map<unsigned char, float> cf) {
	std::vector<unsigned char> c;

	std::map<unsigned char, std::vector<bool>> tree = buildTree(cf);

	std::vector<bool> bols;
	for (auto b : data) {
		bols.push_back(b);
		
		for (std::map<unsigned char, std::vector<bool>>::iterator it = tree.begin(); it != tree.end(); ++it) {
			
			if (it->second.size() == bols.size() and std::equal(it->second.begin(), it->second.end(), bols.begin())) {
				c.push_back(it->first);
				bols.clear();
				break;
			}
		}
	}

	return c;
}