#include "Settings.h"
#include "Graph.h"
#include "ListInt.h"
#include "ShortestPath.h"
#include "Bin_Heap.h"
int main() {
    //TIME CLOCK INITIALISATION --------------------------------
    clock_t start = 0, middle = 0, end = 0;
    double cpu_time_used = 0;
    start = clock();
    //----------------------------------------------------------

    FILE* pfile = fopen("../TPF_Donnees/Tests/1_Dijkstra/input5.txt", "r");
    if (!pfile) {
        return 0;

    }
    AssertNew(pfile);

    char path1[100];
    char path2[100];

    int r =fscanf(pfile, "%[^\n]\n", path1);
    r =fscanf(pfile, "%[^\n]\n", path2);
    int n1, n2;
    r = fscanf(pfile, "%d %d", &n1, &n2);

    printf("%s\n%s\n%d %d\n\n", path1, path2, n1, n2);

    Graph *graph = Graph_load("../TPF_Donnees/Data/france_graph.txt");
    //printf("%d", graph->size);

    Path* p = Binary_Graph_shortestPath(graph, n1, n2);
    Path_print(p);

    





    




    //----------------------------------------------------------

    fclose(pfile);
    Graph_destroy(graph);
    //TIME CLOCK END --------------------------------------------
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("____________________________\n"
        "\nTemps d'execution : %.3fs.\n", cpu_time_used);
    //----------------------------------------------------------

    return EXIT_SUCCESS;
}
