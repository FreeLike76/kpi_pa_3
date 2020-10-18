#include "Ant.h"


void Ant::calcAntPath(Graph& graph)
{
	_pathLen = 0;
	for (int i = 0; i < pathHist.size()-1; i++)
	{
		_pathLen += graph.adj[pathHist[i]][pathHist[i + 1]];
	}
	_pathLen += graph.adj[pathHist.back()][pathHist.front()];
}

int Ant::getPathLen()
{
	return _pathLen;
}

void Ant::clearMemory()
{
	_pathLen = 0;
	pathHist.clear();
}

Ant::~Ant()
{
	clearMemory();
}
