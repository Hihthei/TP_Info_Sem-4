#ifndef GEOJSON_H
#define GEOJSON_H

#include "FileFonction.h"

typedef struct Coord Coord;
typedef struct CoordTab CoordTab;

struct Coord {
	int nb;

	CoordTab* tab;
};

struct CoordTab {
	float latitude;

	float longitude;
};

Coord* Print_createTab(char* fileName);

void Print_loadTab(FILE* pfile, Coord* coord);

void Print_writeGeoJson();

#endif
