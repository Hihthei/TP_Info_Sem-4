#include "Graph.h"

Graph *Graph_create(int size)
{
    Graph *graph = (Graph *)calloc(1, sizeof(Graph));
    AssertNew(graph);

    graph->size = size;

    graph->nodeList = (GraphNode *)calloc(size, sizeof(GraphNode));
    AssertNew(graph->nodeList);

    for (int i = 0; i < size; i++)
    {
        GraphNode *node = &(graph->nodeList[i]);

        node->arcList = NULL;
        node->arcCount = 0;
    }

    return graph;
}

void Graph_destroy(Graph *graph)
{
    if (!graph) return;

    if (graph->nodeList)
    {
        for (int i = 0; i < graph->size; i++)
        {
            GraphNode *node = &(graph->nodeList[i]);
            ArcList *currArc = node->arcList;
            while (currArc != NULL)
            {
                ArcList *nextArc = currArc->next;
                free(currArc);
                currArc = nextArc;
            }
        }
        free(graph->nodeList);
    }

    free(graph);
}

void Graph_setArc(Graph *graph, int u, int v, float weight)
{
    assert(graph);
    assert(0 <= u && u < graph->size);
    assert(0 <= v && v < graph->size);

    // Recherche si l'arc existe
    ArcList *currArc = graph->nodeList[u].arcList;
    while (currArc != NULL)
    {
        if (currArc->target == v)
            break;

        currArc = currArc->next;
    }

    if (currArc)
    {
        // L'arc est déjà présent, on modifie ses données
        currArc->weight = weight;
    }
    else
    {
        // L'arc n'existe pas dans le graphe
        // On l'ajoute en début de list
        currArc = (ArcList *)calloc(1, sizeof(ArcList));
        AssertNew(currArc);

        currArc->next = graph->nodeList[u].arcList;
        currArc->target = v;
        currArc->weight = weight;

        graph->nodeList[u].arcList = currArc;

        graph->nodeList[u].arcCount++;
    }
}

void Graph_removeArc(Graph *graph, int u, int v)
{
    assert(graph);
    assert(0 <= u && u < graph->size);
    assert(0 <= v && v < graph->size);

    // Recherche si l'arc existe
    ArcList *prevArc = NULL;
    ArcList *currArc = graph->nodeList[u].arcList;
    while (currArc != NULL)
    {
        if (currArc->target == v)
            break;

        prevArc = currArc;
        currArc = currArc->next;
    }

    if (currArc)
    {
        // On supprime l'arc
        if (prevArc)
        {
            prevArc->next = currArc->next;
        }
        else
        {
            graph->nodeList[u].arcList = currArc->next;
        }
        free(currArc);

        graph->nodeList[u].arcCount--;
    }
}

float *Graph_getArc(Graph *graph, int u, int v)
{
    assert(graph);
    assert(0 <= u && u < graph->size);
    assert(0 <= v && v < graph->size);

    ArcList *currArc = graph->nodeList[u].arcList;
    while (currArc)
    {
        if (currArc->target == v)
            break;

        currArc = currArc->next;
    }

    if (currArc)
        return &(currArc->weight);

    return NULL;
}


Graph *Graph_load(char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        assert(false);
        return NULL;
    }

    Graph *graph = NULL;

    int scanCount, size, arcCount;
    scanCount = fscanf(file, "%d %d", &size, &arcCount);
    assert(scanCount == 2);

    graph = Graph_create(size);
    for (int i = 0; i < arcCount; i++)
    {
        int u, v;
        float weight;

        scanCount = fscanf(file, "%d %d %f", &u, &v, &weight);
        assert(scanCount == 3);

        Graph_setArc(graph, u, v, weight);
    }

    fclose(file);

    return graph;
}

void Graph_print(Graph *graph)
{
    assert(graph);

    int size = graph->size;
    for (int currID = 0; currID < size; currID++)
    {
        printf("Noeud %d : ", currID);
        for (ArcList *arc = Graph_getArcList(graph, currID);
             arc != NULL; arc = arc->next)
        {
            printf("(tgt %d, wgt %.2f), ", arc->target, arc->weight);
        }
        printf("\n");
    }
}

void Graph_printMoodle(Graph* graph)
{
    assert(graph);

    int size = graph->size;
    for (int currID = 0; currID < size; currID++) {
        for (int i = 0; i < size; i++) {
            if (currID == i) {
                printf("%d %d 0.0\n", currID, i);
                continue;
            }

            float* tmp = Graph_getArc(graph, currID, i);
            
            if (tmp) {
                printf("%d %d %.1f\n", currID, i, *tmp);
            }
        }
    }
}
