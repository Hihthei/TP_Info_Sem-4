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

	free(under_graph);
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

void Sous_Graph_save_path(FILE* pfile, Path* path) {
	AssertNew(pfile);
	AssertNew(path);
	AssertNew(path->list);

	ListInt_print(path->list);

	ListInt* copy_list = ListInt_copy(path->list);

	fprintf(pfile, "%d ", copy_list->nodeCount);

	while (!ListInt_isEmpty(copy_list)) {
		fprintf(pfile, "%d ", ListInt_popFirst(copy_list));
	}

	fprintf(pfile, "\n");
}

UnderGraph* Sous_Graph_load(char* fileName) {
	FILE* pfile = fopen(fileName, "r");
	if (pfile == NULL)
	{
		assert(false);
		return NULL;
	}

	UnderGraph* under_graph = NULL;

	int node_count = 0, arc_count = 0;

	int tmp = fscanf(pfile, "%d %d\n", &node_count, &arc_count);

	under_graph = Sous_Graph_create(node_count);

	int u = 0, v = 0, start = 0, list_value = 0, node_list_count = 0;
	float dist = 0.0f;

	for (int i = 0; i < arc_count; i++) {
		u = 0; v = 0; start = 0; dist = 0.0f; list_value = -1; node_list_count = 0;

		tmp = fscanf(pfile, "%d %d %f %d", &u, &v, &dist, &node_list_count);

		if (u == v) {
			Path* path = Path_create(0);
			path->distance = 0.0f;
			tmp = ListInt_popFirst(path->list);

			under_graph->sous_graph[u][v] = path;

			continue;
		}

		ListInt* list = ListInt_create();

		for (int j = 0; j < node_list_count; j++) {
			tmp = fscanf(pfile, "%d", &list_value);
			ListInt_insertLast(list, list_value);
		}

		Path* path = Path_create(start);
		path->distance = dist;
		path->list = list;

		under_graph->sous_graph[u][v] = path;
	}

	return under_graph;
}
