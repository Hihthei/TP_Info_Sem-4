#include "TSP.h"
#include "Sous_Graph.h"

int Generate_Random(Graph* graph, int nodeact) {
	float somme = 0;
	float value = (float)(rand() % (100 + 1));

	ArcList* tmp = graph->nodeList[nodeact].arcList;
	while (somme < value && tmp->next) {
		somme += tmp->weight;
		tmp = tmp->next;
	}
	return tmp->target;
}

Path* Graph_tspFromACO(	Graph* graph, int station, int iterationCount, int antCount,
						float alpha, float beta, float rho, float q) {
	Path* T = Path_create(station);
	int tmp = ListInt_popFirst(T->list);

	Path** Tj = (Path**)calloc(antCount, sizeof(Path*));

	for (int ant = 0; ant != antCount; ant++) {
		Tj[ant] = Path_create(station);
		tmp = ListInt_popFirst(Tj[ant]->list);
	}
	
	//Création du graph pheromones
	Graph* phem = Graph_create(graph->size);
	for (int u = 0; u != phem->size; u++) {
		for (int v = 0; v != phem->size; v++) {
			if (u != v) {
				Graph_setArc(phem, u, v, 1.f);
			}
		}
	}
	for (int i = 0; i != iterationCount; i++) {
		for (int j = 0; j != antCount; j++) {
			Tj[j] = Graph_acoConstructPath(graph, phem, station, alpha, beta);
			if (ListInt_isEmpty(T->list) || T->distance > Tj[j]->distance) {
				T = Tj[j];
			}
		}
		Graph_acoPheromoneGlobalUpdate(phem, rho);
		for (int j = 0; j != antCount; j++) {
			Graph_acoPheromoneUpdatePath(phem, graph, Tj[j], q);
		}
	}
	return T;
}

Path* Graph_tspFromHeuristic(Graph* graph, int station) {
	int prev = station;
	Path* T = Path_create(station);
	int next = -1;
	float nextw = -1;
	ArcList* tmp;
	bool* explored = (bool*)calloc(graph->size, sizeof(bool));
	AssertNew(explored);
	for (int i = 0; i != graph->size-1; i++) {
		next = -1;
		nextw = -1;
		tmp = graph->nodeList[prev].arcList;
		explored[prev] = true;
		while (tmp) {
			if ((next == -1 || tmp->weight < nextw) && !explored[tmp->target]) {
				nextw = tmp->weight;
				next = tmp->target;
			}
			tmp = tmp->next;
		}
		ListInt_insertLast(T->list, next);
		T->distance += nextw;
		prev = next;
		
	}
	
	float *last = Graph_getArc(graph, next, station);
	ListInt_insertLast(T->list, station);
	T->distance += *last;
	//printf("%f\n", T->distance);
	return T;
}


float* Graph_acoGetProbabilities(Graph* graph, Graph* pheromones, int station,
								bool* explored, float alpha, float beta) {
	float* probabilities = (float*)calloc(pheromones->size, sizeof(float));
	AssertNew(probabilities);

	ArcList* arclist = pheromones->nodeList[station].arcList;
	ArcList* tmp;

	float diviseur=0;
	while (arclist) {
		if (explored[arclist->target]) {
			probabilities[arclist->target] = 0.0f;
		}
		else {
			float* dist = Graph_getArc(graph, station, arclist->target);
			float distc = *dist;
			probabilities[arclist->target] = powf(arclist->weight, alpha) * powf(1 / (distc), beta);
			tmp = pheromones->nodeList[station].arcList;
			diviseur = 0;
			while (tmp) {
				if (!explored[tmp->target]) {
					float* tmpdist = Graph_getArc(graph, station, tmp->target);
					float tmpdistc = *tmpdist;
					diviseur += (float)(pow(tmp->weight, alpha) * pow(1 / tmpdistc, beta));
				}
				tmp = tmp->next;
			}
			probabilities[arclist->target] /= diviseur;
		}
		arclist = arclist->next;
	}
	return probabilities;
}



Path* Graph_acoConstructPath(Graph* distances, Graph* pheros,
	int start, float a, float b) {

	srand((unsigned int)time(NULL));
	int prev = start;
	Path* T = Path_create(start);
	int next = start;
	int nextw = -1;
	float pr = 0;
	int r = 0;
	ArcList* tmp=NULL;
	bool* explored = (bool*)calloc(distances->size, sizeof(bool));
	AssertNew(explored);
	explored[0] = true;
	float* prob = (float*)calloc(distances->size, sizeof(float));
	AssertNew(prob);
	
	for (int i = 0; i != distances->size-1; i++) {
		prev = next;
		
		prob = Graph_acoGetProbabilities(distances, pheros, prev, explored, a, b);

		Graph* graph_proba = Graph_create(pheros->size);

		for (int j = 0; j < pheros->size; j++) {
			if (prev == j)
				continue;

			Graph_setArc(graph_proba, prev, j, prob[j]);
		}

		next = Generate_Random(graph_proba, prev);

		explored[next] = true;

		float *tmpw = Graph_getArc(distances, prev, next);
		float tmpwc = *tmpw;
		nextw = (int)tmpwc;
		ListInt_insertLast(T->list, next);
		T->distance += nextw;

		Graph_destroy(graph_proba);
	}
	
	float *last = Graph_getArc(distances, prev, start);
	float tmpc = *last;
	ListInt_insertLast(T->list, start);
	T->distance += tmpc;
	return T;
}

void Graph_acoPheromoneGlobalUpdate(Graph* pheromones, float rho) {
	ArcList* arc;
	for (int i = 0; i != pheromones->size; i++) {
		arc= pheromones->nodeList[i].arcList;
		while (arc) {
			arc->weight = (1 - rho) * arc->weight;
			arc = arc->next;
		}
	}
}

float Sum_path_dist(Path* path, Graph* graph) {
	ListIntNode* sentinel = &(path->list->sentinel);
	ListIntNode* curr = sentinel->next;

	int last_id = 0, curr_id = curr->value;

	float sum_path_dist = 0.0f;

	curr = curr->next;

	while (curr != sentinel) {
		last_id = curr_id;
		curr_id = curr->value;

		float* arc = Graph_getArc(graph, last_id, curr_id);

		if (arc) {
			sum_path_dist += (float)(*arc);
		}

		curr = curr->next;
	}

	return sum_path_dist;
}

void Graph_acoPheromoneUpdatePath(Graph* pheromones, Graph* graph, Path* path, float q) {
	AssertNew(pheromones);
	AssertNew(graph);
	AssertNew(path);

	ListIntNode* sentinel = &(path->list->sentinel);
	ListIntNode* curr = sentinel->next;

	int last_id = 0, curr_id = curr->value;

	float* arc = NULL;

	curr = curr->next;

	while (curr != sentinel) {
		last_id = curr_id;
		curr_id = curr->value;

		arc = Graph_getArc(graph, last_id, curr_id);

		if (arc) {
			Graph_setArc(pheromones, last_id, curr_id, (float)(q / *arc));

			printf("%f - ", q / *arc);
		}

		curr = curr->next;
	}
}
