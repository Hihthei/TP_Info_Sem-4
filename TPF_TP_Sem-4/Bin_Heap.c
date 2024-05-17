#include "Bin_Heap.h"
#include "stdio.h"
Bin_Heap* Bin_Heap_create(int size) {

	Bin_Heap* heap = (Bin_Heap*)calloc(1, sizeof(Bin_Heap));
	AssertNew(heap);
	heap->size = size;
	
	heap->bin_tab = (Bin_Tab_value**)calloc(size, sizeof(Bin_Tab_value*));
	AssertNew(heap->bin_tab);


	for (int i = 0; i != heap->size; i++) {
		heap->bin_tab[i] = (Bin_Tab_value*)calloc(1, sizeof(Bin_Tab_value));
		AssertNew(heap->bin_tab[i]);
	}
	
	
	//Bin_Heap_print(heap);
	return heap;

}

//void Bin_Heap_swap(Bin_Heap* heap,Bin_Tab_value* v1, Bin_Tab_value* v2, int i1, int i2 ){
//	heap->bin_tab[i1] = v2;
//	heap->bin_tab[i2] = v1;
//}


void Bin_Heap_add(Bin_Heap *heap,int index, float value) {

	
	heap->bin_tab[heap->sizeact]->index = index;
	heap->bin_tab[heap->sizeact]->distance = value;
	
	int act = heap->sizeact;

	int ipere = heap->bin_tab[(act - 1) / 2]->index;
	float pere =heap->bin_tab[(act-1)/2]->distance;
	while (value < pere ) {
		
		heap->bin_tab[(act - 1) / 2]->distance = value;
		heap->bin_tab[(act - 1) / 2]->index = index;

		heap->bin_tab[act]->index = ipere;
		heap->bin_tab[act]->distance = pere;


		act = (act - 1) / 2;
		ipere = heap->bin_tab[(act - 1) / 2]->index;
		pere = heap->bin_tab[(act - 1) / 2]->distance;
	}
	heap->sizeact++;	
}

void Bin_Heap_remove(Bin_Heap* heap) {

	//printf("size = %d\n", heap->sizeact);

	heap->sizeact--;
	heap->bin_tab[0]->index = heap->bin_tab[heap->sizeact]->index;
	heap->bin_tab[0]->distance = heap->bin_tab[heap->sizeact]->distance;



	heap->bin_tab[heap->sizeact]->distance =0;
	heap->bin_tab[heap->sizeact]->index = 0;


	int act = 0;
	int newact=0;
	int itmp=0;
	float tmp=0;
	//printf("here\n");
	//printf("%d ", (2 * act + 1 < heap->sizeact && heap->bin_tab[(2 * act) + 1]->distance < heap->bin_tab[act]->distance) );
	//printf("%d\n", (2 * act + 2 < heap->sizeact && heap->bin_tab[(2 * act) + 2]->distance < heap->bin_tab[act]->distance));
	while ((2 * act + 1 < heap->sizeact && heap->bin_tab[(2 * act) + 1]->distance < heap->bin_tab[act]->distance) ||
		   (2 * act + 2 < heap->sizeact && heap->bin_tab[(2 * act) + 2]->distance < heap->bin_tab[act]->distance)){
		//printf("INSIDE\n");
		if (2 * act + 1 >= heap->sizeact) {
			printf("error");
		}
		//printf("%d %d\n", 2 * act + 1, 2 * act + 2);
		if ((2 * act) + 2 >= heap->sizeact) {
			newact = (2 * act) + 1;
		}
		else if ( (heap->bin_tab[(2 * act) + 1]->distance < heap->bin_tab[act]->distance) && 
			(heap->bin_tab[(2 * act) + 1]->distance < heap->bin_tab[(2 * act) + 2]->distance)) {
			//printf("gauche %.1f\n", heap->bin_tab[(2 * act) + 1]->distance);
			
			newact = (2 * act) + 1;
		}
		else {
			//("droite %.1f\n", heap->bin_tab[(2 * act) + 2]->distance);
			newact = (2 * act) + 2;
		}
		
		//printf("new = %d %.1f\n", newact, heap->bin_tab[newact]->distance);
	
		itmp = heap->bin_tab[act]->index;
		tmp = heap->bin_tab[act]->distance;
		
		heap->bin_tab[act]->index = heap->bin_tab[newact]->index;
		heap->bin_tab[act]->distance = heap->bin_tab[newact]->distance;

		heap->bin_tab[newact]->index = itmp;
		heap->bin_tab[newact]->distance = tmp;

		act = newact;
	}
	return ;
}



void Bin_Heap_print(Bin_Heap* heap) {
	for (int i = 0; i < heap->sizeact; i++) {
		printf("(%d,%.1f) ", heap->bin_tab[i]->index, heap->bin_tab[i]->distance);
	}
	printf("\n\n");
	printf("              (%d,%.1f)	\n", heap->bin_tab[0]->index, heap->bin_tab[0]->distance);
	printf("   (%d,%.1f)	     (%d,%.1f)\n", heap->bin_tab[1]->index, heap->bin_tab[1]->distance, heap->bin_tab[2]->index, heap->bin_tab[2]->distance);
	printf("(%d,%.1f)  (%d,%.1f)  (%d,%.1f)  (%d,%.1f)\n", heap->bin_tab[3]->index, heap->bin_tab[3]->distance, heap->bin_tab[4]->index, heap->bin_tab[4]->distance, heap->bin_tab[5]->index, heap->bin_tab[5]->distance, heap->bin_tab[6]->index, heap->bin_tab[6]->distance);

}

void Bin_Heap_destroy(Bin_Heap *heap) {
	for (int l = 0; l != heap->size; l++) {
		free(heap->bin_tab[l]);
	}
	free(heap->bin_tab);
	free(heap);
}




