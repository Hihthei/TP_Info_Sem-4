#include "Graph.h"
#include "ListInt.h"
#include "ShortestPath.h"
#include "TSP.h"
#include "Print_GeoJson.h"

int main() {
    //TIME-CLOCK-INITIALISATION---------------------------------
    clock_t start = 0, middle = 0, end = 0;
    double cpu_time_used = 0;
    start = clock();

    //FILE-READ-------------------------------------------------
    FILE* pfile = fopen("../TPF_Donnees/Tests/1_Dijkstra/input1.txt", "r");
    AssertNew(pfile);

    char path1[124] = "";
    char path2[124] = "";

    fscanf(pfile, "%[^\n]\n", path1);
    fscanf(pfile, "%[^\n]\n", path2);

    int n1 = 0, n2 = 0;
    fscanf(pfile, "%d %d", &n1, &n2);

    fclose(pfile);

    //GRAPH-----------------------------------------------------
    Graph* graph = Graph_load("../TPF_Donnees/Data/laval_graph.txt");
    Coord* coord = Print_createTab("../TPF_Donnees/Data/laval_inter.txt");
    
    Path* p = Graph_shortestPath(graph, n1, n2);
    Path_print(p);

    //FILE-CREATE-----------------------------------------------
    char* fileName = "..\\Output_geojson\\test.geojson";
    if (FileFonction_fileExist(fileName))
        FileFonction_deleteFile(fileName);

    FileFonction_createFile(fileName);

    FileFunction_writeFile(fileName);

    //FREE------------------------------------------------------
    free(graph);

    free(coord->tab);
    free(coord);

    //TIME CLOCK END -------------------------------------------
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("____________________________\n"
        "\nTemps d'execution : %.3fs.\n", cpu_time_used);

    //----------------------------------------------------------
    return EXIT_SUCCESS;
}
