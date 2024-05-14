#include "Print_GeoJson.h"

Coord* Print_createTab(char* fileName) {
	AssertNew(fileName);

	FILE* pfile = fopen(fileName, "r");
	AssertNew(pfile);

	Coord* coord = (Coord*)calloc(1, sizeof(Coord));
	AssertNew(coord);

	int tmp = fscanf(pfile, "%d\n", &coord->nb);

	coord->tab = (CoordTab*)calloc(coord->nb, sizeof(CoordTab));
	AssertNew(coord->tab);
	
	Print_loadTab(pfile, coord);

	fclose(pfile);

	return coord;
}

void Print_destroyCoord(Coord* coord) {
	if (!coord)
		return;

	free(coord->tab);
	coord->tab = NULL;

	free(coord);
	coord = NULL;
}

void Print_loadTab(FILE* pfile, Coord* coord) {
	AssertNew(pfile);
	AssertNew(coord);

	for (int i = 0; i < coord->nb; i++) {
		int tmp = fscanf(pfile, "%f %f\n", &coord->tab[i].latitude, &coord->tab[i].longitude);
	}
}

void Print_writeGeoJson(char* fileName, Path* path, Coord* coord) {
	FILE* pfile = fopen(fileName, "w");
	AssertNew(pfile);
	AssertNew(path);
	AssertNew(path->list);
	AssertNew(coord);


	fprintf(pfile, "{\n"
		"\t\"type\": \"FeatureCollection\",\n"
		"\t\"features\" : [\n"
		"\t\t{\n"
		"\t\t\t\"type\": \"Feature\",\n"
		"\t\t\t\"geometry\" : {\n"
		"\t\t\t\t\"type\": \"LineString\",\n"
		"\t\t\t\t\"coordinates\" : [\n");

	ListIntNode* sentinel = &(path->list->sentinel);
	ListIntNode* curr = sentinel->next;

	while (curr != sentinel->prev) {
		fprintf(pfile, "\t\t\t\t\t[%f, %f],\n", coord->tab[curr->value].latitude, coord->tab[curr->value].longitude);
		curr = curr->next;
	}
	if (curr != sentinel) {
		fprintf(pfile, "\t\t\t\t\t[%f, %f]\n", coord->tab[curr->value].latitude, coord->tab[curr->value].longitude);
	}

	fprintf(pfile, "\t\t\t\t]\n"
		"\t\t\t}\n"
		"\t\t}\n"
		"\t]\n"
		"}\n");

	fclose(pfile);
}
