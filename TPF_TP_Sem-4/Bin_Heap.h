#ifndef Bin_Heap_H
#define Bin_Heap_H

#include "Settings.h"
typedef struct Bin_Heap{
    int size;
    int sizeact;
    float** tab;
    int* pos;
} Bin_Heap;


Bin_Heap* Bin_Heap_create(int size);
void Bin_Heap_add(Bin_Heap* tab, int index, float value);
void Bin_Heap_remove(Bin_Heap* heap);
void Bin_Heap_update(Bin_Heap* heap, int index, float weight);
void Bin_Heap_print(Bin_Heap* tas);
void Bin_Heap_pos_test(Bin_Heap* heap);



/*void Bin_Heap_update(Bin_Heap* heap, int index, float weight){
	heap->tab[heap->pos[index]][1] = weight;
	int act = heap->pos[index];

	//printf("%.0f %.1f\n", heap->tab[heap->pos[index]][0], heap->tab[heap->pos[index]][1]);

	float ipere = heap->tab[(act - 1) / 2][0];
	float pere = heap->tab[(act - 1) / 2][1];
	while (act != 0 && weight < pere) {
		heap->tab[(act - 1) / 2][0] = index;
		heap->tab[(act - 1) / 2][1] = weight;
		heap->pos[index] = (act - 1) / 2;
		heap->tab[act][0] = ipere;
		heap->tab[act][1] = pere;
		heap->pos[(int)ipere] = act;

		act = (act - 1) / 2;
		ipere = heap->tab[(act - 1) / 2][0];
		pere = heap->tab[(act - 1) / 2][1];

	}


	//CAS DEVENU TROP GRAND -> DESCENDRE
	int newact=0;
	float itmp=0;
	float tmp=0;
	while (heap->tab[(2 * act) + 1][0] != -1 || heap->tab[(2 * act) + 2][0] != -1) {
		if (heap->tab[(2 * act) + 1][1] > heap->tab[act][1] && heap->tab[(2 * act) + 2][1] > heap->tab[act][1]) {
			break;
		}
		else if (heap->tab[(2 * act) + 1][1] < heap->tab[(2 * act) + 2][1] || heap->tab[(2 * act) + 2][1]==-1) {
			newact = (2 * act) + +1;
		}
		else {
			newact = (2 * act) + +2;
		}
		itmp = heap->tab[act][0];
		tmp = heap->tab[act][1];

		heap->tab[act][0] = heap->tab[newact][0];
		heap->tab[act][1] = heap->tab[newact][1];
		heap->pos[(int)heap->tab[newact][0]] = act;

		heap->tab[newact][0] = itmp;
		heap->tab[newact][1] = tmp;
		heap->pos[(int)itmp] = newact;

		act = newact;

	}

}*/
#endif