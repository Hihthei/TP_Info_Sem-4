# TP_Info_Sem-4
TP d'info semestre 4

GENERAL :
	
Dans le fichier "Setting.h", il est possible de :

- Lancer une run "BASIC_RUN", soit une run simple avec le split basique ainsi que le bagging sur les instances (Partie obligatoire du TP).

- Lancer une run "BONUS", soit une run permettant de tester les bonus (SAVE et PAINT) avec le split amélioré ainsi que le bagging sur les features (plutôt que sur les instances). Ne pas tester les bonus même temps (le DATASET_MAISON manque d'instances pour le moment).

- Lancer une run "FOR_MOODLE", soit une run permettant d'obtenir un score sur MOODLE (car les sorties sont celles attendues sur moodle).
// (Pour le meilleur résultat obtenu, voir la vidéo contenu dans le dossier. C'est une run avec NOMBRE_ARBRES 50, PRUNNING_THRESHOLD 1.0f, MAX_DEPTH // 25, le split basique et le bagging sur les features soit BAGGING_PROPORTION 1.0f. -> Score : 9.58 / 10)

- Définir les paramètres suivants : CHEMIN_IMAGE_BMP (le chemin d'accès de la nouvelle instance pour DATASET_MAISON), NOMBRE_ARBRES (le nombre d'arbres pour une forêt), PRUNNING_THRESHOLD (le seuil (inclus) au-delà duquel un noeud est considéré être une feuille), MAX_DEPTH (la profondeur maximale des arbres du programme), BAGGING_PROPORTION (la proportion d'instances utilisée pour les arbres).

- Définir si l'on veut utiliser le split initial/amélioré et le bagging initial/amélioré de manière individuelle (On peut activer la version améliorée des deux, de l'un ou de l'autre, ou d'aucun, en même temps.).

BONUS (fonctionnement)
- SAVE : créer un fichier [filename].txt et le rempli avec la randomForest. Il est également possible de charger un fichier [filename].txt (si celui-ci respecte l'indentation requise). C'est une application terminal, il faut suivre les instructions au lancement du programme.
	-> Buffer et test de réponse cohérente mise en place.

- "PAINT" :
- Se rendre dans le dossier "DATASET" puis ouvrir avec Paint l'image "WrittingTest.bmp". Prendre alors un crayon de couleur blanche et dessiner le nombre souhaité. Il faut alors "SAVE AS" l'image à la place de "WrittingTest6" dans le dossier "Dataset" ou tout autre nom à condition que son chemin d'accès soit récupéré et mis à la place de celui présent dans "Settings.h" -> CHEMIN_IMAGE_BMP.
- On lance alors une run en mode "BONUS" -> "DATASET_MAISON" avec MNIST_train comme fichier d'entrainement (dans le "main.c", mettre le chemin d'accès de "MNIST_train" comme définition de "path_train").
- Deux choix sont alors proposés : "Participer au dataset_maison" enregistrera/ajoutera l'image comme nouvelle instance dans le fichier "WrittingDataset.txt" du dossier "Dataset" et lancera alors le programme avec ce dernier fichier comme fichier de test.
- Le second choix permet de ne pas enregistrer l'instance et de l'utiliser (seule) comme fichier de test.

WARNING :
- Un dossier DATASET est inclus dans le projet. Il ne contient pas les datasets de base mais ceux utilisés pour les bonus.
- L'option DATASET_MAISON n'est pas très précise. En effet, elle ne comporte que peu d'instances (pour l'instant). Il serait possible de définir ce dataset comme fichier d'entraînement lorsque celui-ci sera suffisamment rempli.
- La classe du nombre est rentrer est équivalent au nombre lui-même (exemple : écrivez "2" si vous avez dessiner un 2).