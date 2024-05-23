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
			

			Tj[j] = Graph_acoConstructPath(graph, phem, station, alpha, beta);
			if (ListInt_isEmpty(T->list) || T->distance > Tj[j]->distance) {
				T->distance = Tj[j]->distance;
				T->list = ListInt_copy(Tj[j]->list);
				printf("iteration = %d fourmi = %d distance = %f\n", i,j, T->distance);
			}
		}
		Graph_acoPheromoneGlobalUpdate(phem, rho);
		for (int j = 0; j != antCount; j++) {
			Graph_acoPheromoneUpdatePath(phem, Tj[j], q);
		}
	}
	return T;
}


Path* Graph_tspFromACO_Bonus(Graph* graph, Graph *phem ,int station, int iterationCount, int antCount,
	float alpha, float beta, float rho, float q) {
	Path* T = Path_create(station);
	int tmp = ListInt_popFirst(T->list);

	Path** Tj = (Path**)calloc(antCount, sizeof(Path*));
	AssertNew(Tj);
	for (int ant = 0; ant != antCount; ant++) {
		
		Tj[ant] = Path_create(station);
		tmp = ListInt_popFirst(Tj[ant]->list);
	}

	//Création du graph pheromones
	
	for (int i = 0; i != iterationCount; i++) {
		for (int j = 0; j != antCount; j++) {
			
			Tj[j] = Graph_acoConstructPath(graph, phem, station, alpha, beta);
			if (ListInt_isEmpty(T->list) || T->distance > Tj[j]->distance) {
				T->distance = Tj[j]->distance;
				T->list = ListInt_copy(Tj[j]->list);
				printf("iteration = %d fourmi = %d distance = %f\n", i, j, T->distance);
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




Path* Local_Opti(Graph *graph, Path *path){
	int* tab_id = (int*)calloc(graph->size + 1 , sizeof(int));
	AssertNew(tab_id);
	ListIntNode *tmp = &path->list->sentinel;
	tmp = tmp->next;
	for (int i = 0; i != path->list->nodeCount; i++) {
		tab_id[i] = tmp->value;
 		tmp = tmp->next;
	}

	int s = 0;
	float min = -1;
	float tmpa = 0;
	float* v;
	int* tab_res = (int*)calloc(8, sizeof(int));
	AssertNew(tab_res);
	
	for (int n = 0; n+7 < graph->size  ;n+=7) {
		min = -1;
		
		for (int n0 = s; n0 != s + 7; n0++) {
			if (tab_id[n0] != tab_id[s]) {
				for (int n1 = s; n1 != s + 7; n1++) {
					if (tab_id[n1] != tab_id[n0] && tab_id[n1] != tab_id[s]) {
						for (int n2 = s; n2 != s + 7; n2++) {
							if (tab_id[n2] != tab_id[n1] && tab_id[n2] != tab_id[n0] && tab_id[n2] != tab_id[s]) {
								for (int n3 = s; n3 != s + 7; n3++) {
									if (tab_id[n3] != tab_id[n2] && tab_id[n3] != tab_id[n1] && tab_id[n3] != tab_id[n0] && tab_id[n3] != tab_id[s]) {
										for (int n4 = s; n4 != s + 7; n4++) {
											if (tab_id[n4] != tab_id[n3] && tab_id[n4] != tab_id[n2] && tab_id[n4] != tab_id[n1] && tab_id[n4] != tab_id[n0] && tab_id[n4] != tab_id[s]) {
												for (int n5 = s; n5 != s + 7; n5++) {
													if (tab_id[n5] != tab_id[n4] && tab_id[n5] != tab_id[n3] && tab_id[n5] != tab_id[n2] && tab_id[n5] != tab_id[n1] && tab_id[n5] != tab_id[n0] && tab_id[n5] != tab_id[s]) {
														v = Graph_getArc(graph, tab_id[s], tab_id[n0]);
														tmpa = *v;
														v = Graph_getArc(graph, tab_id[n0], tab_id[n1]);
														tmpa += *v;
														v = Graph_getArc(graph, tab_id[n1], tab_id[n2]);
														tmpa += *v;
														v = Graph_getArc(graph, tab_id[n2], tab_id[n3]);
														tmpa += *v;
														v = Graph_getArc(graph, tab_id[n3], tab_id[n4]);
														tmpa += *v;
														v = Graph_getArc(graph, tab_id[n4], tab_id[n5]);
														tmpa += *v;
														v = Graph_getArc(graph, tab_id[n5], tab_id[s + 7]);
														tmpa += *v;
														if (min == -1 || min > tmpa) {
															min = tmpa;
															tab_res[0] = tab_id[s];
															tab_res[1] = tab_id[n0];
															tab_res[2] = tab_id[n1];
															tab_res[3] = tab_id[n2];
															tab_res[4] = tab_id[n3];
															tab_res[5] = tab_id[n4];
															tab_res[6] = tab_id[n5];
															tab_res[7] = tab_id[s + 7];
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		for (int i = 0 ; i != 7; i++) {
			tab_id[s + i] = tab_res[i];
		}
		s += 7;
	}
	
	Path* path_res = Path_create(tab_id[0]);
	float* w;
	for (int i = 1; i != graph->size +1; i++) {
		ListInt_insertLast(path_res->list, tab_id[i]);
		w = Graph_getArc(graph, tab_id[i - 1], tab_id[i]);
		path_res->distance += *w;
	}
	return path_res;
}