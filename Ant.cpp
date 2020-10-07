#include "Ant.h"

Ant::Ant(int size)
{
	vertAvalivle.resize(size, true);
}

void Ant::clearMemory()
{
	vertAvalivle.clear();
	pathHist.clear();
}

Ant::~Ant()
{
	clearMemory();
}
