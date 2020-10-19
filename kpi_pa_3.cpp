#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <ctime>
#include "Graph.h"
#include "Ant.h"

using namespace std;

#define defFile "matrix.txt"
#define defPheromone 1
#define defMaxIter 1000

#define defA 0.5
#define defB 0.5
#define defRo 0.5
#define defAnt 50
#define defLmin 10
#define defSpread true

vector<int> doAntCycle(Graph& graph, vector<Ant>& allAnts, double A, double B, double Ro, int Lmin);

void spreadAntsT(vector<Ant>& allAnts, int vertCount);
void spreadAntsF(vector<Ant>& allAnts, int vertCount);

int main()
{
	Graph graph(defFile,defPheromone);
	vector<Ant> allAnts(defAnt);

	int Lcur = INFINITY;
	vector<int> Tpath;

	for (int mainIter = 0; mainIter < defMaxIter; mainIter++)
	{
		auto thisPath = doAntCycle(graph, allAnts, defA, defB, defRo, defLmin);
	}
}


vector<int> doAntCycle(Graph &graph,vector<Ant> &allAnts,double A,double B,double Ro,int Lmin)
{
	if (defSpread)
	{
		spreadAntsT(allAnts, graph.size());
	}
	else
	{
		spreadAntsF(allAnts, graph.size());
	}
	for (int antIter = 0; antIter < allAnts.size(); antIter++)
	{
		vector<int> avalVert(graph.size(), true);
		avalVert[allAnts[antIter].pathHist.front()] = false;
		for (int vertIter = 0; vertIter < graph.size(); vertIter++)
		{



		}
	}
	//find min
	//return vector {pathLen,vert1,vert2,...,vertN}
}

void spreadAntsT(vector<Ant> &allAnts,int vertCount)
{
	vector<int> Spread;
	Spread.reserve(vertCount);
	for (int i = 0; i < vertCount; i++)
	{
		Spread.push_back(i);
	}
	random_shuffle(Spread.begin(), Spread.end());
	for (int i = 0; i < allAnts.size(); i++)
	{
		allAnts[i].pathHist.push_back(Spread[i]);
	}
	Spread.clear();
}

void spreadAntsF(vector<Ant>& allAnts, int vertCount)
{
	int a = rand() % (vertCount+1);
	for (int i = 0; i < allAnts.size(); i++)
	{
		allAnts[i].pathHist.push_back(a);
	}
}
