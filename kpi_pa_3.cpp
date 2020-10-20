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

#define defA 3
#define defB 2
#define defRo 0.3
#define defAnt 50
#define defLmin 10
#define defSpread true

int doAntCycle(Graph& graph, vector<Ant>& allAnts, double A, double B, double Ro, int Lmin);

void spreadAntsT(vector<Ant>& allAnts, int vertCount);
void spreadAntsF(vector<Ant>& allAnts, int vertCount);
int evalNextVert(Graph& graph, int curVert, vector<bool>& avalVert, double A, double B);
void pheromoneDecay(Graph& graph, double Ro);
void printDetails(Ant& ant);
void addPheromone(Graph& graph, vector<Ant>& allAnts, int Lmin);
void antReset(vector<Ant>& allAnts);


int main()
{
	srand(time(NULL));
	Graph graph(defFile, defPheromone);
	vector<Ant> allAnts(defAnt);

	int Lcur = INFINITY;
	vector<int> Tpath;

	for (int mainIter = 0; mainIter < defMaxIter; mainIter++)
	{
		int bestAntID = doAntCycle(graph, allAnts, defA, defB, defRo, defLmin);
		pheromoneDecay(graph, defRo);
		addPheromone(graph, allAnts, defLmin);
		if (mainIter % 50 == 1)
			printDetails(allAnts[bestAntID]);

		if (Lcur > allAnts[bestAntID].getPathLen())
		{
			Lcur = allAnts[bestAntID].getPathLen();
			Tpath.clear();
			Tpath = allAnts[bestAntID].pathHist;
		}
		antReset(allAnts);
	}
}


int doAntCycle(Graph &graph,vector<Ant> &allAnts,double A,double B,double Ro,int Lmin)
{
	if (defSpread)
	{
		spreadAntsT(allAnts, graph.size());
	}
	else
	{
		spreadAntsF(allAnts, graph.size());
	}
	for (int antID = 0; antID < allAnts.size(); antID++)
	{
		vector<bool> avalVert(graph.size(), true);
		avalVert[allAnts[antID].pathHist.front()] = false;
		for (int pathIter = 1; pathIter < graph.size(); pathIter++)
		{
			int nextVert = evalNextVert(graph, allAnts[antID].pathHist.back(), avalVert, A, B);
			allAnts[antID].pathHist.push_back(nextVert);
			avalVert[nextVert] = false;
		}
		allAnts[antID].calcAntPath(graph);
	}
	int minPath = allAnts[0].getPathLen();
	int minAntID = 0;
	for (int i = 1; i < allAnts.size(); i++)
	{
		if(minPath>allAnts[i].getPathLen())
		{
			minPath = allAnts[i].getPathLen();
			minAntID = i;
		}
	}
	return minAntID;
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

int evalNextVert(Graph &graph,int curVert, vector<bool> &avalVert,double A,double B)
{
	double totalPrice=0;
	for (int i = 0; i < avalVert.size(); i++)
	{
		if(avalVert[i])
			totalPrice += pow(graph.pheromone[curVert][i], A) * pow((double(1) / double(graph.adj[curVert][i])), B);
	}
	int requiredChance = rand() % 100;
	double chance = 0;
	for (int i = 0; i < avalVert.size(); i++)
	{
		if(avalVert[i])
		{
			chance += (pow(graph.pheromone[curVert][i], A) * pow((double(1) / double(graph.adj[curVert][i])), B) * 100) / totalPrice;
			if (chance > requiredChance)
				return i;
		}
	}
}

void pheromoneDecay(Graph &graph, double Ro)
{
	for (int i = 0; i < graph.size(); i++)
	{
		for (int j = 0; j < graph.size(); j++)
		{
			graph.pheromone[i][j] *= (1 - Ro);
		}
	}
}

void printDetails(Ant& ant)
{
	cout << "Min Length: " << ant.getPathLen() << endl << " Path: ";
	for (int i = 0; i < ant.pathHist.size(); i++)
	{
		cout << ant.pathHist[i] << " ";
	}
	cout << ant.pathHist[0] << endl;
}

void addPheromone(Graph &graph, vector<Ant> &allAnts,int Lmin)
{
	for (int antID = 0; antID < allAnts.size(); antID++)
	{
		double addP = double(Lmin) / double(allAnts[antID].getPathLen());
		for (int i = 0; i < allAnts[antID].pathHist.size() - 1; i++)
		{
			graph.pheromone[allAnts[antID].pathHist[i]][allAnts[antID].pathHist[i + 1]] += addP;
		}
		graph.pheromone[allAnts[antID].pathHist.back()][allAnts[antID].pathHist.front()] += addP;
	}
}

void antReset(vector<Ant> &allAnts)
{
	for (int i = 0; i < allAnts.size(); i++)
	{
		allAnts[i].clearMemory();
	}
}