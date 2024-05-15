#include "Sous_Graph.h"

int main() {
    //TIME-CLOCK-INITIALISATION---------------------------------
    clock_t start = 0, middle = 0, end = 0;
    double cpu_time_used = 0;
    start = clock();

    //FILE-READ-------------------------------------------------
    FILE* pfile = NULL;

    char path_graph[128] = "";
    char path_inter[128] = "";

    Graph* graph_plan = NULL;
    Coord* coord_plan = NULL;
    Path* path = NULL;

    int tmp = 0, i = 0, j = 0;
    
#ifdef DIJKSTRA_1 // tas binaire
    pfile = fopen("../TPF_Donnees/1_Dijkstra/input2.txt", "r");
    AssertNew(pfile);

    tmp = fscanf(pfile, "%[^\n]\n", path_graph);
    tmp = fscanf(pfile, "%[^\n]\n", path_inter);

    int debut = 0, fin = 0;
    tmp = fscanf(pfile, "%d %d", &debut, &fin);

    //GRAPH-----------------------------------------------------
    graph_plan = Graph_load(path_graph);
    coord_plan = Print_createTab(path_inter);

    path = Binary_Graph_shortestPath(graph_plan, debut, fin);

    Path_print(path);

    #ifdef FILE_CREATE
        //FILE-CREATE-----------------------------------------------
        char* fileName = "..\\Output_geojson\\Dijkstra.geojson";
        if (FileFonction_fileExist(fileName))
            FileFonction_deleteFile(fileName);

        FileFonction_createFile(fileName);

        Print_writeGeoJson(fileName, path, coord);
    #endif // FILE_CREATE
#endif // DIJKSTRA_1

#ifdef PATH_MATRIX_2
    if (pfile)
        fclose(pfile);

    pfile = fopen("../TPF_Donnees/2_Path_matrix/input1.txt", "r");
    AssertNew(pfile);

    tmp = fscanf(pfile, "%[^\n]\n", path_graph);
    tmp = fscanf(pfile, "%[^\n]\n", path_inter);

    int node_count = 0;

    tmp = fscanf(pfile, "%d\n", &node_count);

    //GRAPH-----------------------------------------------------
    if (graph_plan)
        Graph_destroy(graph_plan);

    if (coord_plan)
        Print_destroyCoord(coord_plan);

    graph_plan = Graph_load(path_graph);
    coord_plan = Print_createTab(path_inter);

    Graph* graph_matrix = Graph_create(node_count);

    int* tab_node = (int*)calloc(node_count, sizeof(int));

    UnderGraph* under_graph = Sous_Graph_create(node_count);

    if (!path)
        Path_destroy(path);

    for (i = 0; i < node_count; i++)
        tmp = fscanf(pfile, "%d", &tab_node[i]);

    for (i = 0; i < node_count; i++) {
        for (j = 0; j < node_count; j++) {
            if (i == j)
                continue;

            path = Binary_Graph_shortestPath(graph_plan, tab_node[i], tab_node[j]);

            if (Graph_getArc(graph_matrix, i, j) == NULL && path != NULL)
                Graph_setArc(graph_matrix, i, j, path->distance);

            under_graph->sous_graph[i][j] = path;
        }
    }

    //classic_output
    //Graph_print(graph_matrix);
    
    //output_full
    //Sous_Graph_print(under_graph);
    
    Graph_print(graph_matrix);

    //Path*pat = Graph_tspFromHeuristic(graph_matrix, 0);

    //Path_print(pat);


    Graph* phem = Graph_create(graph_matrix->size);
    for (int u = 0; u != phem->size; u++) {
        for (int v = 0; v != phem->size; v++) {
            if (u != v) {
                Graph_setArc(phem, u, v, 1.f);
            }
        }
    }

    Graph_print(phem);

    bool* explored = (bool*)calloc(graph_matrix->size, sizeof(bool));
    AssertNew(explored);
    explored[0] = true;

    float* prob = (float*)calloc(graph_matrix->size, sizeof(float));
    AssertNew(prob);
    prob = Graph_acoGetProbabilities(graph_matrix, phem, 0, explored, 1, 1);

    for (int i = 0; i != graph_matrix->size;i++) {
        printf("%.2f ", prob[i]);
    }
    printf("\n");
    Path* patate = Graph_acoConstructPath(graph_matrix, phem,0, 1, 1);

    Path_print(patate);
    //FREE------------------------------------------------------
    free(tab_node);

    Sous_Graph_destroy(under_graph);

    Graph_destroy(graph_matrix);
    
#endif // PATH_MATRIX_2

    //FREE------------------------------------------------------
    if (graph_plan) {
        Graph_destroy(graph_plan);
        graph_plan = NULL;
    }

    if (coord_plan && coord_plan->tab) {
        Print_destroyCoord(coord_plan);
        coord_plan = NULL;
    }

    if (!path) {
        Path_destroy(path);
        path = NULL;
    }

    if (pfile)
        fclose(pfile);

    //TIME CLOCK END -------------------------------------------
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("____________________________\n"
        "\nTemps d'execution : %.3fs.\n", cpu_time_used);

    //----------------------------------------------------------
    return EXIT_SUCCESS;
}
