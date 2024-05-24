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

    //Créer un output [name].geojson qui est utilisable et visualisable sur le site umap
    #define FILE_CREATE

    //Réponses correspondantes au format demandé sur moodle
    //#define FOR_MOODLE
    
    //Lance un algorithme Dijkstra supplémenté d'un tas binaire
    #define DIJKSTRA_1

    //Créer le sous graphe de chemin ainsi que la matrice de chemin
    #define PATH_MATRIX_2

        #ifdef PATH_MATRIX_2
            
            //Charge une matrice pré-enregistrée
            #define LOAD_MATRIX
            //Sauvegarde la matrice créée
            #define PATH_MATRIX_SAVE
            
    //On ne peut pas charger la matrice pour la sauvegarder ensuite, même si les deux définitions sont décommentées

        #endif // PATH_MATRIX_2

    //Lance un algorithme glouton basé sur des Dijkstra binaire
    #define TSP_HEURISTIC_3

    //Lance un algorithme d'ACO
    #define TSP_ACO_4

    //Zone de définition des bonus de l'ACO
    #define TSP_ACO_BONUS

        #ifdef TSP_ACO_BONUS

            //Charge un graph pré-enregistré (même algorithme que le PATH_MATRIX)
            #define TSP_ACO_LOAD_MATRIX
            //Sauvegarde le graph
            #define TSP_ACO_SAVE_MATRIX

    //On ne peut pas charger le graph pour le sauvegarder ensuite

            //Lance un glouton sur un graph de phéromones, accélère le processus de l'ACO
            #define TSP_GLOUTON_ACO

            #ifdef TSP_GLOUTON_ACO
                
                //Lance l'algorithme de l'ACO en testant tout les départs
                #define BONUS_ALL_START
                //Parcours le chemin de retour de l'ACO pour voir s'il n'existe pas de chemin plus adapté (peut affecté l'ordre du chemin)
                #define TSP_OPTI_LOCAL

            #endif //TSP_GLOUTON_ACO
        #endif // TSP_ACO_BONUS

    //Lance en mode grande instance sur la France (compter environ 8 minutes en release pour un Glouton + ACO)
    #define TSP_ACO_GI //Grande Instance -> france

#endif