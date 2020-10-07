#pragma once
#include <vector>

using namespace std;

class Ant
{
public:
	vector<bool> vertAvalivle;
	vector<int> pathHist;
	Ant(int size);
	void clearMemory();
	~Ant();
};

