#include "TSP.h"
#include "Sous_Graph.h"

int Generate_Random(Graph* graph, int nodeact) {

	float somme = 0;
	float value = (float)(rand() % (100 + 1)) / 100;
	
	ArcList* tmpold= NULL;
	ArcList* tmp = graph->nodeList[nodeact].arcList;

	while (somme < value && tmp->next) {
		somme += tmp->weight;
		tmpold = tmp;
		tmp = tmp->next;
	}
	while (tmp->next && tmp->weight==0.f ) {
		tmp = tmp->next;
	}
	if (tmp->weight==0) {
		if (tmpold) {
			tmp = tmpold;
		}
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
			/*ArcList* tmp = Graph_getArcList(phem, station);
			while (tmp) {
				printf("%.2f ",tmp->weight);
				tmp = tmp->next;
			}*/

			Tj[j] = Graph_acoConstructPath(graph, phem, station, alpha, beta);
			if (ListInt_isEmpty(T->list) || T->distance > Tj[j]->distance) {
				T->distance = Tj[j]->distance;
				T->list = ListInt_copy(Tj[j]->list);
				printf("%f ", T->distance);
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

	int prev = start;
	Path* T = Path_create(start);
	int next = start;
	float nextw = -1;
	float pr = 0;
  
	ArcList* tmp=NULL;
	bool* explored = (bool*)calloc(distances->size, sizeof(bool));
	AssertNew(explored);
	explored[0] = true;
	float* prob = (float*)calloc(distances->size, sizeof(float));
	AssertNew(prob);
	
	for (int i = 0; i != distances->size-1; i++) {
		prev = next;
    
		explored[prev] = true;
		prob = Graph_acoGetProbabilities(distances, pheros, prev, explored, a, b);

		Graph* proba = Graph_create(pheros->size);
		for (int i = 0; i != pheros->size;i++) {
				Graph_setArc(proba, prev, i, prob[i]);	
		}
		/*
		tmp = Graph_getArcList(proba, prev);
		while (tmp) {
			printf("%.2f ", prob[tmp->target]);
			tmp = tmp->next;
		}
		printf("\n");*/
		//Graph_print(proba);

		next = Generate_Random(proba, prev);
		//printf("next %d\n", next);
		while (next == prev)
			next = Generate_Random(proba, prev);
		
		float* tmpw = Graph_getArc(distances, prev, next);
		nextw = (float)*tmpw;
		ListInt_insertLast(T->list, next);
		T->distance += nextw;
    
		//ListInt_print(T->list);

		Graph_destroy(proba);
	}
	
	float* last = Graph_getArc(distances, next, start);

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

void Graph_acoPheromoneUpdatePath(Graph* pheromones, Graph* graph, Path* path, float q) {
	AssertNew(pheromones);
	AssertNew(graph);
	AssertNew(path);

	ListIntNode* sentinel = &(path->list->sentinel);
	ListIntNode* curr = sentinel->next;

	int last_id = 0, curr_id = curr->value;

	float* arc = NULL;
	float* arc_bis = NULL;

	curr = curr->next;

	while (curr != sentinel) {
		last_id = curr_id;
		curr_id = curr->value;

		arc_bis = Graph_getArc(pheromones, last_id, curr_id);

		arc = Graph_getArc(graph, last_id, curr_id);

		if (arc && arc_bis) {
			Graph_setArc(pheromones, last_id, curr_id, *arc_bis + (float)(q / *arc));
		}

		curr = curr->next;
	}
}
