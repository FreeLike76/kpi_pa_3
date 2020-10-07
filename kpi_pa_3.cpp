#include <iostream>
#include <vector>
#include <cmath>
#include "Graph.h"

using namespace std;

#define defFile "matrix.txt"

#define defA 1
#define defB 1
#define defRo 0.5
#define defAnt 50
#define defLmin 10
#define defSpread true

int main()
{
	//Choosing a parameter to test
	auto testParam = defA;
	Graph graph(defFile);
	vector < vector <int> > pheromone(6, std::vector<int>(6, 1));
}
