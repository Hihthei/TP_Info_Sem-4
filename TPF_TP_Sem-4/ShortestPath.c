#include "ShortestPath.h"
#include "Bin_Heap.h"
Path *Graph_shortestPath(Graph *graph, int start, int end)
{
    int size = Graph_size(graph);

    assert(0 <= start && start < size);
    assert(end < size);

    int *predecessors = (int *)calloc(size, sizeof(int));
    AssertNew(predecessors);

    float *distances = (float *)calloc(size, sizeof(float));
    AssertNew(distances);

    Graph_dijkstra(graph, start, end, predecessors, distances);

    Path *path = Graph_dijkstraGetPath(predecessors, distances, end);

    free(predecessors);
    predecessors = NULL;
    free(distances);
    distances = NULL;

    return path;
}

Path* Binary_Graph_shortestPath(Graph* graph, int start, int end)
{
    int size = Graph_size(graph);

    assert(0 <= start && start < size);
    assert(end < size);

    int* predecessors = (int*)calloc(size, sizeof(int));
    AssertNew(predecessors);

    float* distances = (float*)calloc(size, sizeof(float));
    AssertNew(distances);

    Binary_Graph_dijkstra(graph, start, end, predecessors, distances);

    Path* path = Graph_dijkstraGetPath(predecessors, distances, end);

    free(predecessors);
    predecessors = NULL;
    free(distances);
    distances = NULL;

    return path;
}

void Graph_dijkstra(Graph *graph, int start, int end, int *predecessors, float *distances)
{
    int size = Graph_size(graph);

    assert(0 <= start && start < size);
    assert(end < size);

    bool *explored = (bool *)calloc(Graph_size(graph), sizeof(bool));
    AssertNew(explored);

    for (int i = 0; i < size; i++)
    {
        predecessors[i] = -1;
        distances[i] = INFINITY;
    }
    distances[start] = 0.0f;



    for(int n =0;n!=graph->size;n++)
    {
        // Recherche le noeud de distance minimale
        int currID = -1;
        float currDist = INFINITY;
        for (int i = 0; i < size; ++i)
        {
            if (explored[i] == false && distances[i] < currDist)
            {
                currDist = distances[i];
                currID = i;
            }
        }
        // Condition d'arret
        if (currID < 0 || currID == end)
            break;

        explored[currID] = true;

        // Met à jour les voisins
        for (ArcList *arc = Graph_getArcList(graph, currID);
             arc != NULL; arc = arc->next)
        {
            int nextID = arc->target;
            float dist = distances[currID] + arc->weight;
            if (distances[nextID] > dist)
            {
                distances[nextID] = dist;
                predecessors[nextID] = currID;
            }
        }
    }
    free(explored);
}

void Binary_Graph_dijkstra(Graph* graph, int start, int end, int* predecessors, float* distances)
{
    int size = Graph_size(graph);

    assert(0 <= start && start < size);
    assert(end < size);

    bool* explored = (bool*)calloc(Graph_size(graph), sizeof(bool));
    AssertNew(explored);

    for (int i = 0; i < size; i++)
    {
        predecessors[i] = -1;
        distances[i] = INFINITY;
    }
    distances[start] = 0.0f;

    Bin_Heap* heap = Bin_Heap_create(graph->size);
    Bin_Heap_add(heap, start, 0.f);
  
    //Bin_Heap_print(heap);
  
    for (int n = 0; n < graph->size; n++)
    {
        // Recherche le noeud de distance minimale

        int currID = (int)heap->tab[0][0];
        float currDist = heap->tab[0][1];

        Bin_Heap_remove(heap);
        while (explored[currID]) {
            currID = (int)heap->tab[0][0];
            currDist = heap->tab[0][1];
            Bin_Heap_remove(heap);
        }
        // Condition d'arret
        if (currID < 0 || currID == end)
            break;

        explored[currID] = true;

        // Met à jour les voisins
        for (ArcList* arc = Graph_getArcList(graph, currID);
            arc != NULL; arc = arc->next)
        {
            int nextID = arc->target;
            float dist = distances[currID] + arc->weight;
            if (distances[nextID] > dist){
                distances[nextID] = dist;
                predecessors[nextID] = currID;
                Bin_Heap_add(heap, nextID, dist);   
            }
        }
    }
    free(explored);
    Bin_Heap_destroy(heap);
}

Path *Graph_dijkstraGetPath(int *predecessors, float *distances, int end)
{
    assert(predecessors && distances);
    assert(end >= 0);

    if (predecessors[end] < 0)
    {
        return NULL;
    }

    Path *path = Path_create(end);
    int currID = end;

    while (predecessors[currID] >= 0)
    {
        currID = predecessors[currID];
        ListInt_insertFirst(path->list, currID);
    }

    path->distance = distances[end];

    return path;
}

Path *Path_create(int start)
{
    Path *path = (Path *)calloc(1, sizeof(Path));
    AssertNew(path);

    path->distance = 0.0f;
    path->list = ListInt_create();
    ListInt_insertLast(path->list, start);

    return path;
}

void Path_destroy(Path *path)
{
    if (path == NULL) return;

    if (!ListInt_isEmpty(path->list))
        ListInt_destroy(path->list);
    else
        free(path->list);
    
    free(path);
}

void Path_print(Path *path)
{
    if (path == NULL)
    {
        printf("path : NULL\n");
        return;
    }

    printf("path (distance = %.1f) : ", path->distance);
    ListInt_print(path->list);
}
