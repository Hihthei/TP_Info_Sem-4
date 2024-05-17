#include "TSP.h"
#include "Sous_Graph.h"

int Generate_Random(Graph* graph, int nodeact) {
	ArcList* tmp = graph->nodeList[nodeact].arcList;

	// Vérification de la liste d'arcs pour le nœud actuel
	if (!tmp) {
		return -1;  // Aucun arc disponible
	}

	// Calculer la somme totale des poids
	float total_weight = 0;
	ArcList* iter = tmp;
	while (iter) {
		total_weight += iter->weight;
		iter = iter->next;
	}

	// Gérer le cas où tous les poids sont nuls
	if (total_weight == 0) {
		return tmp->target;  // Retourner n'importe quel arc, ici le premier
	}

	// Générer une valeur aléatoire entre 0 et total_weight
	float random_value = ((float)rand() / RAND_MAX) * total_weight;

	// Sélectionner l'arc basé sur la valeur aléatoire
	float cumulative_weight = 0;
	iter = tmp;
	while (iter) {
		cumulative_weight += iter->weight;
		if (cumulative_weight >= random_value) {
			return iter->target;
		}
		iter = iter->next;
	}

	// Par défaut, retourner le dernier nœud cible, même si cela ne devrait pas arriver
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
			}
			printf("\n");*/
			//Graph_print(phem);
			//printf("\n");

			Tj[j] = Graph_acoConstructPath(graph, phem, station, alpha, beta);
			if (ListInt_isEmpty(T->list) || T->distance > Tj[j]->distance) {
				T->distance = Tj[j]->distance;
				T->list = ListInt_copy(Tj[j]->list);
				printf("%f ", T->distance);
			}
		}
		Graph_acoPheromoneGlobalUpdate(phem, rho);
		for (int j = 0; j != antCount; j++) {
			Graph_acoPheromoneUpdatePath(phem, Tj[j], q);
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
			
			probabilities[arclist->target] = powf(arclist->weight, alpha) * powf(1 / (*dist), beta);
			tmp = pheromones->nodeList[station].arcList;
			diviseur = 0;
			while (tmp) {
				if (!explored[tmp->target]) {
					float* tmpdist = Graph_getArc(graph, station, tmp->target);
					diviseur += (float)(powf(tmp->weight, alpha) * powf(1 / *tmpdist, beta));
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

	float* prob = (float*)calloc(distances->size, sizeof(float));
	AssertNew(prob);
	
	for (int i = 0; i != distances->size-1; i++) {
		prev = next;
    
		explored[prev] = true;
		prob = Graph_acoGetProbabilities(distances, pheros, prev, explored, a, b);

		Graph* proba = Graph_create(pheros->size);
		for (int i = 0; i != pheros->size;i++) {
			if(!explored[i])
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
			printf("inside");
			next = Generate_Random(proba, prev);
		
		float* nextw = Graph_getArc(distances, prev, next);
		
		ListInt_insertLast(T->list, next);
		T->distance += *nextw;
    
		Graph_destroy(proba);
	}
	
	float* last = Graph_getArc(distances, next, start);
	
	ListInt_insertLast(T->list, start);
	T->distance += *last;
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

void Graph_acoPheromoneUpdatePath(Graph* pheromones, Path* path, float q){
	ArcList* arc;
	for (int i = 0; i != pheromones->size; i++) {
		arc = pheromones->nodeList[i].arcList;
		while (arc) {
			arc->weight = arc->weight + (q / path->distance);
			arc = arc->next;
		}
	}
}
