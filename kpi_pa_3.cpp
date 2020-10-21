#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <ctime>
#include <fstream>
#include <chrono>
#include "Graph.h"
#include "Ant.h"

using namespace std;

#define defFile "matrix.txt"
#define defPheromone 1
#define defMaxIter 100
#define defRequiredLen 1600
#define defStep 1

//_test1 minValue to test
//_test2 maxValue to test
//_best best velue, founded in testing

#define defA_test1 0
#define defA_test2 10
#define defA_best 2
//
#define defB_test1 0
#define defB_test2 10
#define defB_best 3
//
#define defRo_test1 0.1
#define defRo_test2 0.9
#define defRo_best 0.3
//
#define defAnt_test1 10
#define defAnt_test2 300
#define defAnt_best 50
//
#define defLmin_test1 100
#define defLmin_test2 5000
#define defLmin_best 1000
//
#define defSpread_test1 false
#define defSpread_test2 true
#define defSpread_best true

int findPathUntil(int RequiredPath, int A, int B, int Ro, int Lmin, int AntCount, bool Spread);
int doAntCycle(Graph& graph, vector<Ant>& allAnts, double A, double B, double Ro, int Lmin, bool Spread);

void spreadAntsT(vector<Ant>& allAnts, int vertCount);
void spreadAntsF(vector<Ant>& allAnts, int vertCount);
int evalNextVert(Graph& graph, int curVert, vector<bool>& avalVert, double A, double B);
void pheromoneDecay(Graph& graph, double Ro);
void addPheromone(Graph& graph, vector<Ant>& allAnts, int Lmin);
void antReset(vector<Ant>& allAnts);
void csvWriter(string path, double testParam, pair<int, float> details);

int main()
{
	string testOutputFile = "A";
	srand(time(NULL));
	//defRequiredLen, defA_best, defB_best, defRo_best, defLmin_best, defAnt_best, defSpread_best

	for (auto testParam = defA_test1; testParam < defA_test2; testParam += defStep)
	{
		auto start = chrono::high_resolution_clock::now();
		int tookIter = findPathUntil(defRequiredLen, testParam, defB_best, defRo_best, defLmin_best, defAnt_best, defSpread_best);
		auto end = chrono::high_resolution_clock::now();
		chrono::duration<float> tookTime = end - start;
		csvWriter(testOutputFile, testParam,make_pair(tookIter,tookTime.count()));
	}
	testOutputFile = "B";
	for (auto testParam = defB_test1; testParam < defB_test2; testParam += defStep)
	{
		auto start = chrono::high_resolution_clock::now();
		int tookIter = findPathUntil(defRequiredLen, defA_best, testParam, defRo_best, defLmin_best, defAnt_best, defSpread_best);
		auto end = chrono::high_resolution_clock::now();
		chrono::duration<float> tookTime = end - start;
		csvWriter(testOutputFile, testParam, make_pair(tookIter, tookTime.count()));
	}
}


int findPathUntil(int RequiredPath, int A, int B, int Ro, int Lmin, int AntCount, bool Spread)
{
	auto start = chrono::high_resolution_clock::now();
	Graph graph(defFile, defPheromone);
	vector<Ant> allAnts(AntCount);

	int Lcur;
	//vector<int> Tpath;
	int lock_counter = 0;

	for (int mainIter = 0; mainIter < defMaxIter; mainIter++)
	{
		int bestAntID = doAntCycle(graph, allAnts, A, B, Ro, Lmin, Spread);
		pheromoneDecay(graph, Ro);
		addPheromone(graph, allAnts, Lmin);
		if (mainIter == 0 || Lcur > allAnts[bestAntID].getPathLen())
		{
			Lcur = allAnts[bestAntID].getPathLen();
			/*Tpath.clear();
			Tpath = allAnts[bestAntID].pathHist;*/
			lock_counter = 0;
		}
		else
			lock_counter++;
		if (lock_counter > 20)
			break;
		cout << "Min Path Len: " << Lcur << endl;
		antReset(allAnts);
		if(Lcur<defRequiredLen)
		{
			return mainIter;
		}
	}
	return defMaxIter;
}

int doAntCycle(Graph & graph, vector<Ant> & allAnts, double A, double B, double Ro, int Lmin, bool Spread)
	{
		if (Spread)
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
			if (minPath > allAnts[i].getPathLen())
			{
				minPath = allAnts[i].getPathLen();
				minAntID = i;
			}
		}
		return minAntID;
	}

void spreadAntsT(vector<Ant> & allAnts, int vertCount)
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

void spreadAntsF(vector<Ant> & allAnts, int vertCount)
	{
		int a = rand() % (vertCount + 1);
		for (int i = 0; i < allAnts.size(); i++)
		{
			allAnts[i].pathHist.push_back(a);
		}
	}

int evalNextVert(Graph & graph, int curVert, vector<bool> & avalVert, double A, double B)
	{
		double totalPrice = 0;
		for (int i = 0; i < avalVert.size(); i++)
		{
			if (avalVert[i])
				totalPrice += pow(graph.pheromone[curVert][i], A) * pow((double(1) / double(graph.adj[curVert][i])), B);
		}
		int requiredChance = rand() % 100;
		double chance = 0;
		for (int i = 0; i < avalVert.size(); i++)
		{
			if (avalVert[i])
			{
				chance += (pow(graph.pheromone[curVert][i], A) * pow((double(1) / double(graph.adj[curVert][i])), B) * 100) / totalPrice;
				if (chance > requiredChance)
					return i;
			}
		}
	}

void pheromoneDecay(Graph & graph, double Ro)
	{
		for (int i = 0; i < graph.size(); i++)
		{
			for (int j = 0; j < graph.size(); j++)
			{
				graph.pheromone[i][j] *= (1 - Ro);
			}
		}
	}

void addPheromone(Graph & graph, vector<Ant> & allAnts, int Lmin)
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

void antReset(vector<Ant> & allAnts)
	{
		for (int i = 0; i < allAnts.size(); i++)
		{
			allAnts[i].clearMemory();
		}
	}


void csvWriter(string path, double testParam, pair<int, float> details)
{
	cout << testParam << '\t' << details.first << '\t' << details.second << endl;
	fstream a;
	a.open("csvResults\\" + path + "_iter.csv", ios::app);
	a << testParam << '\t' << details.first << endl;
	a.close();

	a.open("csvResults\\" + path + "_time.csv", ios::app);
	a << testParam << '\t' << details.second << endl;
	a.close();
}
