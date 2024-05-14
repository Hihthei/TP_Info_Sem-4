#ifndef UNDER_GRAPH_H
#define UNDER_GRAPH_H

	#include "FileFonction.h"

	typedef struct UnderGraph UnderGraph;

	struct UnderGraph {
		int node_count;

		Path*** sous_graph;
	};

	UnderGraph* Sous_Graph_create(int node_count);

	void Sous_Graph_destroy(UnderGraph* under_graph);

	void Sous_Graph_print(UnderGraph* under_graph);

#endif
