#include "Huffman.h"
#include "Utility.h"


//template class Huffman<int>;
//template class Huffman<float>;

/// <summary>
/// Za vsak znak dobimo njegov binarni zapis
/// </summary>
/// <typeparam name="T">float - verjetnostna tabela, int - �tevilo pojavitev</typeparam>
/// <param name="v">znaki z njihovimi podatki</param>
/// <param name="root">koren</param>
/// <param name="b">binarni vektor</param>
template<class T>
void Huffman<T>::makeCodes(std::map<char, std::vector<bool>>& v, struct Node* root, std::vector<bool> b)
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

/// <summary>
/// Zgradimo Huffmanovo dravo
/// </summary>
/// <typeparam name="T">float - verjetnostna tabela, int - �tevilo pojavitev</typeparam>
/// <param name="v">verjetnostna tabela / tabela, ki �teje pojavitve</param>
/// <returns></returns>
template<class T>
std::map<char, std::vector<bool>> Huffman<T>::buildTree(std::map<char, float> v)
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

		top = new Node(std::make_pair<std::optional<char>, T>({}, l->p.second + r->p.second));

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
template<class T>
std::map<char, float> Huffman<T>::calculateProbability(std::map<char, int> mp, int size)
{
	std::map<char, float> cf;

	for (auto p : mp) {
		if (p.second != 0) {
			cf.insert(std::pair<char, float>(p.first, static_cast<float>(static_cast<double>(p.second) / size)));
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
template<class T>
std::tuple<std::map<char, std::vector<bool>>, std::map<char, float>> Huffman<T>::Encode(std::vector<char>* v)
{
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

	// std::cout << "--------------DEBUG INFO--------------" << std::endl;
	// for (auto it : mp)  {
	// 	std::cout << static_cast<int>(it.first) << "\t" << it.second << std::endl; 
	// }
	// std::cout << "--------------DEBUG INFO--------------" << std::endl;


	//mp.erase(std::remove_if(mp.begin(), mp.end(), [](std::pair<char, int> p) { return p.second != 0; }), mp.end());

	//std::vector<std::pair<char, int>> vec = sortedVectorOfValues(mp);

	std::map<char, float> cf = calculateProbability(mp, v->size());

	std::map<char, std::vector<bool>> tree = buildTree(cf);

	// for (auto node : tree) {
	// 	std::cout << static_cast<int>(node.first) << "\t";
	// 	for (auto b : node.second) {
	// 		std::cout << b;
	// 	}
	// 	std::cout <<std::endl;
	// }

	//std::map<char, float> cf = calculateProbability(mp, v->size());

	return std::tuple<std::map<char, std::vector<bool>>, std::map<char, float>>(tree, cf);
}

/// <summary>
/// Huffmanovo dekodiranje
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="data">binarni podatki</param>
/// <param name="cf">verjetnostna tabela</param>
/// <returns>slikovni podatki</returns>
template<class T>
std::vector<char>* Huffman<T>::Decode(std::vector<bool>* data, std::map<char, float> cf) {
	std::vector<char>* c = new std::vector<char>;

	std::map<char, std::vector<bool>> tree = buildTree(cf);

	// for (auto node : tree) {
	// 	std::cout << static_cast<int>(node.first) << "\t";
	// 	for (auto b : node.second) {
	// 		std::cout << b;
	// 	}
	// 	std::cout << std::endl;
	// }

	std::vector<bool> bols;
	for (auto b : *data) {
		bols.push_back(b);
		
		for (std::map<char, std::vector<bool>>::iterator it = tree.begin(); it != tree.end(); ++it) {
			
			if (it->second.size() == bols.size() and std::equal(it->second.begin(), it->second.end(), bols.begin())) {
				c->push_back(it->first);
				bols.clear();
				break;
			}
		}
	}

	return c;
}

//void TemopraryFunction() {
//	Huffman<int>hf1;
//	Huffman<float>hf2;
//}