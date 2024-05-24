# TPF_TP_Sem-4
# TP d'info semestre 4

## GENERAL :
	
- Dans le fichier "Setting.h", il est possible de :

- Commenter ou décommenter les parties de codes que nous souhaitons tester (il est possible de tester tout en même temps)

|      FILE_CREATE : Créer un output [name].geojson qui est utilisable et visualisable sur le site umap.
|                    Ce fichier est créé dans le dossier 'Output_geojson'

|      FOR_MOODLE : Réponses correspondants au format demandé sur moodle
      
|      DIJKSTRA_1 :  Lance un algorithme Dijkstra supplémenté d'un tas binaire
•                    -> Compatible avec une création de fichier

|      PATH_MATRIX_2 : Créer le sous graphe de chemin ainsi que la matrice de chemin {
•                    -> Compatible avec une sauvegarde

|            LOAD_MATRIX : Charge une matrice pré-enregistrée    -\
•                                                                  |-> La charge de la matrice prendre le dessus vis-à-vis de la sauvegarde.
|            PATH_MATRIX_SAVE : Sauvegarde la matrice créée      -/    Il n'est pas possible de sauvegarder un graph précédemment chargé.
      }

|      TSP_HEURISTIC_3 : Lance un algorithme glouton basé sur des algorithmes de Dijkstra (binaire)
•                        -> Compatible avec une création de fichier

|      TSP_ACO_4 : Lance un algorithme d'ACO
•                  -> Compatible avec une création de fichier


# TSP_ACO_BONUS : Zone de définition des bonus de l'ACO {
•                 -> Compatible avec une création de fichier
•                 -> Compatible avec une sauvegarde

|      TSP_ACO_LOAD_MATRIX : Charge un graph pré-enregistré (même algorithme que le PATH_MATRIX)   -\
•                                                                                                    |-> La charge de la matrice prendre le dessus vis-à-vis de la sauvegarde.
|      TSP_ACO_SAVE_MATRIX : Sauvegarde le graph de l'aco                                          -/    Il n'est pas possible de sauvegarder un graph précédemment chargé.

|      TSP_GLOUTON_ACO : Lance un glouton sur un graph de phéromones, accélère le processus de l'ACO

|      BONUS_ALL_START : Lance l'algorithme de l'ACO en testant tout les départs

|      TSP_OPTI_LOCAL : Parcours le chemin de retour de l'ACO pour voir s'il n'existe pas de chemin plus adapté (peut affecté l'ordre du chemin)

|      TSP_ACO_GI : Lance en mode grande instance sur la France (compter environ 8 minutes en release pour un Glouton + ACO)
}

|BONUS (fonctionnement)
|- LOAD : ouvre un fichier spécial (stocké dans Personnal_Data), crée ensuite un graphe à partir de la fonction Graph_Load()
|- SAVE : crée un fichier spécifique (toujours dans Personnal_Data). Noter qu'on ne peut pas load et save en même temps
|- GLOUTON_ACO : lance un algorithme glouton sur un graphe de phéromones, diminue le nombre d'instances totales nécessaires pour déterminer un bon chemin
|- OPTI_LOCAL : lance des permutations entre les différentes points du chemin pour tester différentes versions. Peut trouver un meilleur chemin

|WARNING :
|Pour le bon fonctionnement du programme, il est demandé que les fichiers répondent à l'implémentation suivante :

_________________________________________
|                                        |
|Data/                                   |
|    |_*.txt                             |
|                                        |
|TPF_Donnees/                            |
|    |_1_Dijkstra/                       |
|        |_*.txt                         |
|    |_2_Path_Matrix/                    |
|        |_*.txt                         |
|    |_3_TSP_Heuristic/                  |
|        |_*.txt                         |
|    |_4_TSP_ACO/                        |
|        |_*.txt                         |
|    |_5_Grande_instance/                |
|        |_*.txt                         |
|                                        |
|Personal_Data/                          |
|    |_2_Path_matrix                     |
|        |_*.txt                         |
|    |_4_TSP_ACO                         |
|        |_*.txt                         |
|                                        |
|.sln                                    |
|________________________________________|
