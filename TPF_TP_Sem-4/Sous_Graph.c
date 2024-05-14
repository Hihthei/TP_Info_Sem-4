#include "Sous_Graph.h"

UnderGraph* Sous_Graph_create(int node_count) {
	assert(node_count >= 0);

	UnderGraph* under_graph = (UnderGraph*)calloc(1, sizeof(UnderGraph));
	AssertNew(under_graph);

	under_graph->node_count = node_count;

	under_graph->sous_graph = (Path***)calloc(node_count, sizeof(Path**));
	AssertNew(under_graph->sous_graph);

	for (int i = 0; i < node_count; i++) {
		under_graph->sous_graph[i] = (Path**)calloc(node_count, sizeof(Path*));
		AssertNew(under_graph->sous_graph[i]);
	}

	return under_graph;
}

void Sous_Graph_destroy(UnderGraph* under_graph) {
	if (!under_graph)
		return;

	for (int i = 0; i < under_graph->node_count; i++) {
		for (int j = 0; j < under_graph->node_count; j++) {
			Path_destroy(under_graph->sous_graph[i][j]);
			under_graph->sous_graph[i][j] = NULL;
		}
		free(under_graph->sous_graph[i]);
		under_graph->sous_graph[i] = NULL;
	}
	free(under_graph->sous_graph);
	under_graph->sous_graph = NULL;
}

void Sous_Graph_print(UnderGraph* under_graph) {
	if (!under_graph)
		return;

	for (int i = 0; i < under_graph->node_count; i++) {
		for (int j = 0; j < under_graph->node_count; j++) {
			if (i == j) {
				printf("%d %d 0.0 0\n", i, j);

				continue;
			}

			if (under_graph->sous_graph[i][j] != NULL) {
				printf("%d %d %.1f ", i, j, under_graph->sous_graph[i][j]->distance);
				ListInt_print(under_graph->sous_graph[i][j]->list);
			}
		}
		printf("\n\n");
	}
}
