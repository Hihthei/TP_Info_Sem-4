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



    //TIME CLOCK END --------------------------------------------
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("____________________________\n"
        "\nTemps d'execution : %.3fs.\n", cpu_time_used);
    //----------------------------------------------------------

    return EXIT_SUCCESS;
}
