#include "FileFonction.h"

void CodeError_FILE(void** freeptr, char* errormsg) {
	if (errormsg != NULL)
		printf("\nCODE ERROR : \" %s \" \n\n", errormsg);

	if (freeptr != NULL && *freeptr != NULL) {
		free(*freeptr);
		*freeptr = NULL;
	}
}

//FONCTION : -----------------------------------------------------------------------------------------------------------

bool FileFonction_fileExist(char* fileName) {
	AssertNew(fileName);

	FILE* pfile = NULL;
	pfile = fopen(fileName, "r");

	if (pfile == NULL)
		return false;
	else {
		fclose(pfile);
		return true;
	}
}

void FileFonction_fileOverwrite(char* fileName) {
	AssertNew(fileName);

	FileFonction_deleteFile(fileName);
	FileFonction_createFile(fileName);

	return;
}

void FileFonction_createFile(char* fileName) {
	AssertNew(fileName);

	char command[512] = "";

	sprintf(command, "ECHO.> %s", fileName);

	system(command);
}

void FileFonction_deleteFile(char* fileName) {
	AssertNew(fileName);

	if (!FileFonction_fileExist(fileName))
		return;

	char command[512] = "";

	sprintf(command, "DEL %s", fileName);

	system(command);
}

void FileFunction_writeFile(char* fileName, Path* path, Coord* coord) {
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
		fprintf(pfile,"\t\t\t\t\t[%f, %f],\n", coord->tab[curr->value].latitude, coord->tab[curr->value].longitude);
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
