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
	double** pheromone;
	Graph(string path,double _pheromone);
	~Graph();
	int size();
};

