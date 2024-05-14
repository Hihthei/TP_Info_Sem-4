#include "Bin_Heap.h"
#include "stdio.h"
Bin_Heap* Bin_Heap_create(int size) {

	Bin_Heap* heap = (Bin_Heap*)calloc(1, sizeof(Bin_Heap));
	AssertNew(heap);
	heap->size = size;
	heap->tab = (float**)calloc(size, sizeof(float*));
	heap->pos = (int*)calloc(size, sizeof(int));
	AssertNew(heap->tab);
	
	for (int i = 0; i != heap->size; i++) {
		heap->tab[i] = (float*)calloc(2 , sizeof(float));
		AssertNew(heap->tab[i]);
	}
	
	Bin_Heap_print(heap);
	//Bin_Heap_pos_test(heap);
	return heap;

}
void Bin_Heap_add(Bin_Heap *heap,int index, float value) {
	heap->tab[heap->sizeact][0] = index;
	heap->tab[heap->sizeact][1] = value;
	heap->pos[heap->sizeact] = index;

	int act = heap->sizeact;

	float ipere = heap->tab[(act - 1) / 2][0];
	float pere =heap->tab[(act-1)/2][1];
	while (act != 0 && value < pere ) {
		heap->tab[(act - 1) / 2][0] = index;
		heap->tab[(act - 1) / 2][1] = value;
		heap->pos[index] = (act - 1) / 2;
		heap->tab[act][0] = ipere;
		heap->tab[act][1] = pere;
		heap->pos[(int)ipere] = act;

		act = (act - 1) / 2;
		ipere = heap->tab[(act - 1) / 2][0];
		pere = heap->tab[(act - 1) / 2][1];
	}
	heap->sizeact++;	
}



void Bin_Heap_remove(Bin_Heap* heap) {
	heap->sizeact--;
	heap->tab[0][0] = heap->tab[heap->sizeact][0];
	heap->tab[0][1] = heap->tab[heap->sizeact][1];
	heap->pos[heap->sizeact] = 0;

	heap->tab[heap->sizeact][0] = 0;
	heap->tab[heap->sizeact][1] = 0;
	int act = 0;
	int newact=0;
	float itmp=0;
	float tmp=0;
	
	while (heap->tab[(2*act)+1][1] < heap->tab[act][1] || heap->tab[(2*act)+2][1] < heap->tab[act][1]) {
		if ((2 * act) + 2 >= heap->sizeact) {
			//printf("supp\n");
			break;
		}
		if (heap->tab[(2 * act) + 1][1] < heap->tab[(2 * act) + 2][1]) {
			newact = (2 * act) + 1;
		}
		else {
			newact = (2 * act) + 2;
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
	return ;
}
void Bin_Heap_print(Bin_Heap* heap) {
	for (int i = 0; i < heap->sizeact; i++) {
		printf("(%.1f,%.1f) ", heap->tab[i][0], heap->tab[i][1]);
	}
	printf("\n");
	printf("              (%.0f,%.1f)	\n", heap->tab[0][0], heap->tab[0][1]);
	printf("   (%.0f,%.1f)	     (%.0f,%.1f)\n", heap->tab[1][0], heap->tab[1][1], heap->tab[2][0] , heap->tab[2][1]);
	printf("(%.0f,%.1f)  (%.0f,%.1f)  (%.0f,%.1f)  (%.0f,%.1f)\n", heap->tab[3][0], heap->tab[3][1], heap->tab[4][0], heap->tab[4][1], heap->tab[5][0], heap->tab[5][1], heap->tab[6][0], heap->tab[6][1]);

}

void Bin_Heap_pos_test(Bin_Heap* heap) {
	for (int i = 0; i < heap->sizeact; i++) {
		//printf("(%.1f,%.1f)", heap->tab[heap->pos[i]][0], heap->tab[heap->pos[i]][1]);
		if ((int)heap->tab[heap->pos[i]][0] != i) {
			printf("error %d %.0f\n", i, heap->tab[heap->pos[i]][0]);
		}
	}
}

