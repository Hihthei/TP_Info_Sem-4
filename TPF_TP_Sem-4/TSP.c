#include "TSP.h"


Path* Graph_tspFromHeuristic(Graph* graph, int station){

	int prev = station;
	Path* T = Path_create(station);
	int next = -1;
	float nextw = -1;
	for (int i = 0; i != graph->size; i++) {
		ArcList* tmp = graph->nodeList[prev].arcList;
		next = -1;
		nextw = -1;
		while (tmp) {
			if (next == -1 || tmp->weight < nextw) {
				nextw = tmp->weight;
				next = tmp->target;
			}
			tmp = tmp->next;
		}
		ListInt_insertLast(T->list, next);
		T->distance += nextw;
		prev = next;
	}
	ArcList* tmp = Graph_getArc(graph, next, station);

	ListInt_insertLast(T->list, station);
	T->distance += tmp->weight;
	return T;
}


float* Graph_acoGetProbabilities(
	Graph* graph, Graph* pheromones, int station,
	bool* explored, float alpha, float beta){
	float* probabilities = (float*)calloc(pheromones->size, sizeof(float));
	AssertNew(probabilities);
	ArcList* arclist = pheromones->nodeList[station].arcList;
	ArcList* dist;
	ArcList* tmp;
	ArcList* tmpdist;
	float diviseur=0;
	while (arclist) {
		if (explored[arclist->target]) {
			probabilities[arclist->target] = 0.0f;
		}
		else {
			dist = Graph_getArc(graph, station, arclist->target);
			probabilities[arclist->target] = powf(arclist->weight, alpha) * (1 / dist->weight, beta);
			tmp = pheromones->nodeList[station].arcList;
			while (tmp) {
				tmpdist = Graph_getArc(graph, station, tmp->target);
				diviseur += pow(tmp->weight, alpha) * pow(1 / tmpdist->weight, beta);
				tmp = tmp->next;
			}
		}
	}

	return probabilities;
}

Path* Graph_acoConstructPath(Graph* distances, Graph* pheros,
	int start, float a, float b) {
	srand(time(NULL));
	int prev = start;
	Path* T = Path_create(start);
	int next = -1;
	int nextw = -1;
	float pr = 0;
	ArcList* tmpw;
	for (int i = 0; i != distances->size; i++) {
		ArcList* tmp = distances->nodeList[prev].arcList;
		next = -1;
		nextw = -1;
		int r = rand() % (100 + 1);
		pr = 0.0f;
		while (tmp && r < pr) {
			pr += tmp->weight;
			tmp = tmp->next;
		}
		next = tmp->target;
		tmpw = Graph_getArc(distances, prev, tmp->target);
		nextw = tmpw->weight;
		ListInt_insertLast(T->list, next);
		T->distance += nextw;
		prev = next;
	}
	ArcList* tmp = Graph_getArc(distances, next, start);

	ListInt_insertLast(T->list, start);
	T->distance += tmp->weight;
	return T;
}



/*Path* Graph_tspFromACO(Graph* graph, int s, float a, float b, float p, float q, int n, int k) {
	Path* T = (Path*)calloc(1, sizeof(Path));
	Graph* P = (Graph*)calloc(1, sizeof(Graph));
	for (int i = 0; i != n; i++) {
		for (int j = 0; j != k; j++) {

		}
	}



	return NULL;
}*/