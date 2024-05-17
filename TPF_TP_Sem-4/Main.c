#include "Sous_Graph.h"
#include "Bin_Heap.h"

void free_graph(Graph** graph) {
    if (*graph) {
        Graph_destroy(*graph);
        *graph = NULL;
    }
}

void free_coord(Coord** coord) {
    if (*coord) {
        Print_destroyCoord(*coord);
        *coord = NULL;
    }
}

void free_path(Path** path) {
    if (*path) {
        Path_destroy(*path);
        *path = NULL;
    }
}

void free_undergraph(UnderGraph** under_graph) {
    if (*under_graph) {
        Sous_Graph_destroy(*under_graph);
        *under_graph = NULL;
    }
}

int main() {
    //TIME-CLOCK-INITIALISATION---------------------------------
    clock_t start = 0, middle = 0, end = 0;
    double cpu_time_used = 0;
    start = clock();
    srand((unsigned int)time(NULL));
  
    //----------------------------------------------------------
    FILE* pfile = NULL;

    char path_graph[128] = "";
    char path_inter[128] = "";

    Graph* graph_plan = NULL;
    Coord* coord_plan = NULL;
    Path* path = NULL;

    char fileName[256] = "";

    int tmp = 0, i = 0, j = 0;
    
#ifdef DIJKSTRA_1 // tas binaire
    pfile = fopen("../TPF_Donnees/1_Dijkstra/input1.txt", "r");
    AssertNew(pfile);

    tmp = fscanf(pfile, "%[^\n]\n", path_graph);
    tmp = fscanf(pfile, "%[^\n]\n", path_inter);

    int tab[2] = { 0 };
    tmp = fscanf(pfile, "%d %d", &tab[0], &tab[1]);

    //GRAPH-----------------------------------------------------
    graph_plan = Graph_load(path_graph);
    coord_plan = Print_createTab(path_inter);

    path = Binary_Graph_shortestPath(graph_plan, tab[0], tab[1]);

    #ifdef FOR_MOODLE
        printf("%.1f\n", path->distance);
        printf("%d\n", path->list->nodeCount);
        ListInt_print(path->list);

    #else
        Path_print(path);

        #ifdef FILE_CREATE
                //FILE-CREATE-----------------------------------------------
                strcpy(fileName, "..\\Output_geojson\\Dijkstra.geojson");
                if (FileFonction_fileExist(fileName))
                    FileFonction_deleteFile(fileName);

                FileFonction_createFile(fileName);

                //Print_writeGeoJson(fileName, path, coord_plan);
                Print_writeGeoJson_Bonus(fileName, path, coord_plan, tab, 2);

        #endif // FILE_CREATE
    #endif // FOR_MOODLE

    //FREE------------------------------------------------------
    free_graph(&graph_plan);

    free_coord(&coord_plan);

    free_path(&path);

    fclose(pfile);
    pfile = NULL;
#endif // DIJKSTRA_1

#ifdef PATH_MATRIX_2
    pfile = fopen("../TPF_Donnees/2_Path_matrix/input1.txt", "r");
    if (!pfile)
        pfile = stdin;

    AssertNew(pfile);

    tmp = fscanf(pfile, "%[^\n]\n", path_graph);
    tmp = fscanf(pfile, "%[^\n]\n", path_inter);

    int node_count = 0;

    tmp = fscanf(pfile, "%d\n", &node_count);

    //GRAPH-----------------------------------------------------
    graph_plan = Graph_load(path_graph);
    coord_plan = Print_createTab(path_inter);

    Graph* graph_matrix = Graph_create(node_count);
    AssertNew(graph_matrix);
    
    int* tab_node = (int*)calloc(node_count, sizeof(int));
    AssertNew(tab_node);
    UnderGraph* under_graph = Sous_Graph_create(node_count);
    AssertNew(under_graph);

    for (i = 0; i < node_count; i++)
        tmp = fscanf(pfile, "%d", &tab_node[i]);

    for (i = 0; i < node_count; i++) {
        for (j = 0; j < node_count; j++) {
            if (i == j)
                printf("%d %d 0.0\n", i, j);
            else {
                path = Binary_Graph_shortestPath(graph_plan, tab_node[i], tab_node[j]);
                printf("%d %d %.1f\n", i, j, path->distance);
            }
        }
    }
  
    #ifdef FOR_MOODLE
        Graph_printMoodle(graph_matrix);
    #else
        //classic_output
        Graph_print(graph_matrix);

        //full_output
        Sous_Graph_print(under_graph);
    #endif // FOR_MOODLE

    //FREE------------------------------------------------------
    free_graph(&graph_plan);
    free_graph(&graph_matrix);

    Sous_Graph_destroy(under_graph);
    under_graph = NULL;

    path = NULL;

    free_coord(&coord_plan);
    
    fclose(pfile);
    pfile = NULL;

    free(tab_node);
#endif // PATH_MATRIX_2

#ifdef TSP_HEURISTIC_3
    pfile = fopen("../TPF_Donnees/3_TSP_Heuristic/input1.txt", "r");
    AssertNew(pfile);

    tmp = fscanf(pfile, "%[^\n]\n", path_graph);
    tmp = fscanf(pfile, "%[^\n]\n", path_inter);

    int node_count_heuristic = 0;

    tmp = fscanf(pfile, "%d\n", &node_count_heuristic);

    //GRAPH-----------------------------------------------------
    graph_plan = Graph_load(path_graph);
    coord_plan = Print_createTab(path_inter);

    Graph* graph_heuristic = Graph_create(node_count_heuristic);

    UnderGraph* under_graph_heuristic = Sous_Graph_create(node_count_heuristic);

    int* tab_node_heuristic = (int*)calloc(node_count_heuristic, sizeof(int));

    for (i = 0; i < node_count_heuristic; i++)
        tmp = fscanf(pfile, "%d", &tab_node_heuristic[i]);

    for (i = 0; i < node_count_heuristic; i++) {
        for (j = 0; j < node_count_heuristic; j++) {
            if (i == j)
                continue;

            path = Binary_Graph_shortestPath(graph_plan, tab_node_heuristic[i], tab_node_heuristic[j]);

            if (Graph_getArc(graph_heuristic, i, j) == NULL && path != NULL)
                Graph_setArc(graph_heuristic, i, j, path->distance);

            under_graph_heuristic->sous_graph[i][j] = path;
        }
    }

    //HEURISTIC-------------------------------------------------
    path = Graph_tspFromHeuristic(graph_heuristic, 0);

    #ifdef FOR_MOODLE
        printf("%.1f %d\n", path->distance, path->list->nodeCount);
        ListInt_print(path->list);

    #else
        Path_print(path);

        #ifdef FILE_CREATE
            Path* complet_path = NULL;

            if (!ListInt_isEmpty(path->list)) {
                complet_path = Path_create(tab_node_heuristic[0]);

                int prev = 0, current = ListInt_popFirst(path->list);

                while (!ListInt_isEmpty(path->list)) {
                    prev = current;

                    current = ListInt_popFirst(path->list);

                    ListInt* heuristic_list = under_graph_heuristic->sous_graph[prev][current]->list;

                    if (!ListInt_isEmpty(heuristic_list))
                        tmp = ListInt_popFirst(heuristic_list);

                    ListInt_concatenate(complet_path->list, heuristic_list);
                }
            }

            //FILE-CREATE-----------------------------------------------
            strcpy(fileName, "..\\Output_geojson\\TSP_Heuristic.geojson");
            if (FileFonction_fileExist(fileName))
                FileFonction_deleteFile(fileName);

            FileFonction_createFile(fileName);

            Print_writeGeoJson_Bonus(fileName, complet_path, coord_plan, tab_node_heuristic, node_count_heuristic);
        
        #endif // FILE_CREATE
    #endif // FOR_MOODLE

    //FREE------------------------------------------------------
    free_graph(&graph_plan);
    free_graph(&graph_heuristic);

    free_coord(&coord_plan);

    free_path(&path);
    free_path(&complet_path);

    fclose(pfile);
    pfile = NULL;

    free(tab_node_heuristic);
#endif // TSP_HEURISTIC

#ifdef TSP_ACO_4
    pfile = fopen("../TPF_Donnees/4_TSP_ACO/input4.txt", "r");
    AssertNew(pfile);

    tmp = fscanf(pfile, "%[^\n]\n", path_graph);
    tmp = fscanf(pfile, "%[^\n]\n", path_inter);

    int node_count_aco = 0;

    tmp = fscanf(pfile, "%d\n", &node_count_aco);

    //GRAPH-----------------------------------------------------
    graph_plan = Graph_load(path_graph);
    coord_plan = Print_createTab(path_inter);

    Graph* graph_aco = Graph_create(node_count_aco);

    UnderGraph* under_graph_aco = Sous_Graph_create(node_count_aco);

    int* tab_node_aco = (int*)calloc(node_count_aco, sizeof(int));

    for (i = 0; i < node_count_aco; i++)
        tmp = fscanf(pfile, "%d", &tab_node_aco[i]);

    for (i = 0; i < node_count_aco; i++) {
        for (j = 0; j < node_count_aco; j++) {
            if (i == j)
                continue;

            path = Binary_Graph_shortestPath(graph_plan, tab_node_aco[i], tab_node_aco[j]);

            if (Graph_getArc(graph_aco, i, j) == NULL && path != NULL)
                Graph_setArc(graph_aco, i, j, path->distance);

            under_graph_aco->sous_graph[i][j] = path;
        }
    }

    //ACO------------------------------------------------------
    path = Graph_tspFromACO(graph_aco, 0, 1000, 100, 2.f, 3.f , 0.1f, 2.0f);
    
    

    /* Path* pa = Path_create(0);
    Graph* grrr = Graph_create(4);

    Graph_setArc(grrr, 0, 1, 1);
    Graph_setArc(grrr, 0, 2, 2);
    Graph_setArc(grrr, 0, 3, 8);

    Graph_setArc(grrr, 1, 0, 1);
    Graph_setArc(grrr, 1, 3, 2);

    Graph_setArc(grrr, 2, 0, 2);
    Graph_setArc(grrr, 2, 3, 2);

    Graph_print(grrr);
    //Graph_acoPheromoneGlobalUpdate(phem, 0.05f);
    ListInt_insertLast(pa->list, 1);
    ListInt_insertLast(pa->list, 3);
    pa->distance = 3;
    Path_print(pa);
    Graph_acoPheromoneUpdatePath(phem, grrr, pa, 2.0f);
    Graph_print(phem);*/

    /*Graph* graph_p = Graph_create(graph_aco->size);
    for (int i = 0; i != graph_aco->size; i++) {
        for (int j = 0; j != graph_aco->size; j++) {
            if (i == j) {
                continue;
            }
            else {
                Graph_setArc(graph_p, i, j, 1);
            }
        }
    }*/


    //path = Graph_acoConstructPath(graph_aco, graph_p, 0, 1.f, 1.f);
    Path_print(path);
    
#ifdef FILE_CREATE_TODO
    Path* complet_path_aco = NULL;

    if (!ListInt_isEmpty(path->list)) {
        complet_path_aco = Path_create(tab_node_aco[0]);

        int prev = 0, current = ListInt_popFirst(path->list);

        while (!ListInt_isEmpty(path->list)) {
            prev = current;
            
            current = ListInt_popFirst(path->list);

            ListInt* aco_list = under_graph_aco->sous_graph[prev][current]->list;

            if (!ListInt_isEmpty(aco_list))
                tmp = ListInt_popFirst(aco_list);

            ListInt_concatenate(complet_path_aco->list, aco_list);
        }
    }

    //FILE-CREATE-----------------------------------------------
    char* fileName = "..\\Output_geojson\\TSP_Aco.geojson";
    if (FileFonction_fileExist(fileName))
        FileFonction_deleteFile(fileName);

    FileFonction_createFile(fileName);

    //Print_writeGeoJson(fileName, complet_path_aco, coord_plan);
    Print_writeGeoJson_Bonus(fileName, complet_path_aco, coord_plan, tab_node_aco, node_count_aco);
#endif // FILE_CREATE

    //FREE------------------------------------------------------
    free_graph(&graph_plan);
    free_graph(&graph_aco);

    free_coord(&coord_plan);

    free_path(&path);

    fclose(pfile);
    pfile = NULL;

    free(tab_node_aco);
#endif // TSP_ACO

    //TIME CLOCK END -------------------------------------------
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("____________________________\n"
        "\nTemps d'execution : %.3fs.\n", cpu_time_used);

    //----------------------------------------------------------
    return EXIT_SUCCESS;
}
