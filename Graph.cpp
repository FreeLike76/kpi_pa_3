#include "Graph.h"

Graph::Graph(string path)
{

	ifstream input;
	input.open(path);
	input >> _size;

	adj = new int* [_size];
	pow.resize(_size, 0);

	for (int i = 0; i < _size; i++)
	{
		adj[i] = new int[_size];
		for (int j = 0; j < _size; j++)
		{
			input >> adj[i][j];
			if (adj[i][j] == 1)
				pow[i]++;
		}
	}
	input.close();
}

Graph::~Graph()
{
	for (int i = 0; i < _size; i++)
	{
		delete[] adj[i];
	}
	delete[] adj;
	adj = nullptr;
	pow.clear();
}

int Graph::size()
{
	return _size;
}