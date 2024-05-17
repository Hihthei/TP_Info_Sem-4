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

    #define TSP_HEURISTIC_3

    #define TSP_ACO_4

#endif