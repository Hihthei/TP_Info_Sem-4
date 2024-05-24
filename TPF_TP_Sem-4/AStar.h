#ifndef ASTAR_H
#define ASTAR_H

	#include "Settings.h"
	#include "Graph.h"
	#include "ListInt.h"
	#include "ShortestPath.h"
	#include "Sous_Graph.h"
	#include "Bin_Heap.h"

	Path* AStar_shortestPath(Graph* graph, Coord* coord, int start, int end);

#endif