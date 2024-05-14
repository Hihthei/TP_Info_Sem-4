#include "Print_GeoJson.h"

Coord* Print_createTab(char* fileName) {
	AssertNew(fileName);

	printf("%s\n", fileName);

	FILE* pfile = fopen(fileName, "r");
	AssertNew(pfile);

	Coord* coord = (Coord*)calloc(1, sizeof(Coord));
	AssertNew(coord);

	fscanf(pfile, "%d\n", &coord->nb);

	coord->tab = (CoordTab*)calloc(coord->nb, sizeof(CoordTab));
	AssertNew(coord->tab);
	
	Print_loadTab(pfile, coord);

	fclose(pfile);

	return coord;
}

void Print_loadTab(FILE* pfile, Coord* coord) {
	AssertNew(pfile);
	AssertNew(coord);

	for (int i = 0; i < coord->nb; i++) {
		fscanf(pfile, "%f %f\n", &coord->tab[i].latitude, &coord->tab[i].longitude);
	}
}

void Print_writeGeoJson() {

}
