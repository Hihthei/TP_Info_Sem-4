#ifndef GEOJSON_H
#define GEOJSON_H

	#include "Settings.h"
	#include "Graph.h"
	#include "ListInt.h"
	#include "ShortestPath.h"
	#include "TSP.h"

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

	void Print_destroyCoord(Coord* coord);

	void Print_loadTab(FILE* pfile, Coord* coord);

	void Print_writeGeoJson(char* fileName, Path* path, Coord* coord);


	//Print with point
	void Print_writeGeoJson_Bonus(char* fileName, Path* path, Coord* coord, int* point, int taille);


#endif
