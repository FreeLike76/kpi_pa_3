#include "Graph.h"

Graph::Graph(string path, double _pheromone)
{

	ifstream input;
	input.open(path);
	input >> _size;

	adj = new int* [_size];
	pheromone = new double* [_size];

	for (int i = 0; i < _size; i++)
	{
		adj[i] = new int[_size];
		pheromone[i] = new double[_size];
		for (int j = 0; j < _size; j++)
		{
			input >> adj[i][j];
			pheromone[i][j] = _pheromone;
		}
		pheromone[i][i] = 0;
	}
	input.close();
}

Graph::~Graph()
{
	for (int i = 0; i < _size; i++)
	{
		delete[] adj[i];
		delete[] pheromone[i];
	}
	delete[] adj;
	delete[] pheromone;
	adj = nullptr;
	pheromone = nullptr;
}

int Graph::size()
{
	return _size;
}