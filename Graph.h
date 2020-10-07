#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class Graph
{
private:
	int _size;
public:
	int** adj;
	vector<int> pow;
	Graph(string path);
	~Graph();
	int size();
};

