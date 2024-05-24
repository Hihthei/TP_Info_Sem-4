#ifndef SETTINGS_H
#define SETTINGS_H

    #include <assert.h>
    #include <math.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <stdint.h>
    #include <time.h>

    #define AssertNew(ptr) { if (ptr == NULL) { assert(false); abort(); } }

    #ifdef _MSC_VER
    #  define INLINE inline
    #else
    #  define INLINE static inline
    #endif

    INLINE int minInt(int a, int b) { return a < b ? a : b; }
    INLINE int maxInt(int a, int b) { return a > b ? a : b; }

    #define FILE_CREATE

    //#define FOR_MOODLE
    
    #define DIJKSTRA_1

    #define PATH_MATRIX_2

        #ifdef PATH_MATRIX_2
            
            #define LOAD_MATRIX // charger une matrice de chemin pré-sauvegardée
            #define PATH_MATRIX_SAVE

        #endif // PATH_MATRIX_2

    #define TSP_HEURISTIC_3

    #define TSP_ACO_4

        #ifdef TSP_ACO_4

            //#define TSP_ACO_GI //Grande Instance -> france

        #endif // TSP_ACO_4

    #define TSP_ACO_BONUS

        #ifdef TSP_ACO_BONUS

            #define BONUS_ALL_START

        #endif // TSP_ACO_BONUS

#endif