#include "AStar.h"

float euclidean_distance(int from, int to, Coord* coord) {
    return 0.0f;
}

Path* AStar_shortestPath(Graph* graph, Coord* coord, int start, int end) {
    int size = Graph_size(graph);
    bool* closedSet = (bool*)calloc(size, sizeof(bool));
    AssertNew(closedSet);

    float* gScore = (float*)calloc(size, sizeof(float));
    AssertNew(gScore);

    float* fScore = (float*)calloc(size, sizeof(float));
    AssertNew(fScore);

    int* predecessors = (int*)calloc(size, sizeof(int));
    AssertNew(predecessors);

    for (int i = 0; i < size; ++i) {
        gScore[i] = INFINITY;
        fScore[i] = INFINITY;
        predecessors[i] = -1;
    }

    gScore[start] = 0.0f;
    fScore[start] = euclidean_distance(start, end, coord);

    Bin_Heap* openSet = Bin_Heap_create(size);
    Bin_Heap_add(openSet, start, fScore[start]);

    while (openSet->sizeact > 0) {
        int current = openSet->bin_tab[0]->index;
        if (current == end) {
            Path* path = Graph_dijkstraGetPath(predecessors, gScore, end);
            free(closedSet);
            free(gScore);
            free(fScore);
            free(predecessors);
            Bin_Heap_destroy(openSet);
            return path;
        }

        Bin_Heap_remove(openSet);
        closedSet[current] = true;

        for (ArcList* arc = Graph_getArcList(graph, current); arc != NULL; arc = arc->next) {
            int neighbor = arc->target;
            if (closedSet[neighbor]) {
                continue;
            }

            float tentative_gScore = gScore[current] + arc->weight;

            if (tentative_gScore < gScore[neighbor]) {
                predecessors[neighbor] = current;
                gScore[neighbor] = tentative_gScore;
                fScore[neighbor] = gScore[neighbor] + euclidean_distance(neighbor, end, coord);
                if (!Bin_Heap_contains(openSet, neighbor)) {
                    Bin_Heap_add(openSet, neighbor, fScore[neighbor]);
                }
            }
        }
    }

    free(closedSet);
    free(gScore);
    free(fScore);
    free(predecessors);
    Bin_Heap_destroy(openSet);
    return NULL;
}
