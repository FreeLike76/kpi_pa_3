#include <iostream>
#include <vector>
#include <cmath>
#include "Graph.h"
#include "Ant.h"

using namespace std;

#define defFile "matrix.txt"
#define defPheromone 1

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
	Graph graph(defFile,defPheromone);
	vector<Ant> allAnts(defAnt);
}