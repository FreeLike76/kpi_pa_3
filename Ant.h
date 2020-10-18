#pragma once
#include <vector>
#include "Graph.h"
using namespace std;

class Ant
{
	int _pathLen=0;
public:
	vector<int> pathHist;

	~Ant();

	void calcAntPath(Graph& graph);
	int getPathLen();
	void clearMemory();
};

