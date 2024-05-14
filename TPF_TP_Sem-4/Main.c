#include "Settings.h"
#include "Graph.h"
#include "ListInt.h"
#include "ShortestPath.h"

int main() {
    //TIME CLOCK INITIALISATION --------------------------------
    clock_t start = 0, middle = 0, end = 0;
    double cpu_time_used = 0;
    start = clock();
    //----------------------------------------------------------

    FILE* pfile = fopen("../TPF_Donnees/Tests/1_Dijkstra/input1.txt", "r");
    if (!pfile) {
        return NULL;

    }
    AssertNew(pfile);

    char path1[100];
    char path2[100];

    fscanf(pfile, "%[^\n]\n", path1);
    fscanf(pfile, "%[^\n]\n", path2);
    int n1, n2;
    fscanf(pfile, "%d %d", &n1, &n2);

   
    fclose(pfile);
    printf("%s\n%s\n%d %d\n", path1, path2, n1, n2);

    Graph *graph = Graph_load("../TPF_Donnees/Data/laval_graph.txt");
    //printf("%d", graph->size);

    Path *p = Graph_shortestPath(graph, n1, n2);
    Path_print(p);

    //----------------------------------------------------------

    fclose(pfile);

    //TIME CLOCK END --------------------------------------------
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("____________________________\n"
        "\nTemps d'execution : %.3fs.\n", cpu_time_used);
    //----------------------------------------------------------

    return EXIT_SUCCESS;
}
