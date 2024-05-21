#ifndef Bin_Heap_H
#define Bin_Heap_H

#include "Settings.h"
typedef struct Bin_Tab_value {
    float distance;
    int index;

}Bin_Tab_value;

typedef struct Bin_Heap{
    int size;
    int sizeact;
    Bin_Tab_value** bin_tab;

} Bin_Heap;


Bin_Heap* Bin_Heap_create(int size);
void Bin_Heap_add(Bin_Heap* heap, int index, float value);
void Bin_Heap_remove(Bin_Heap* heap);
void Bin_Heap_print(Bin_Heap* heap);
void Bin_Heap_destroy(Bin_Heap* heap);
bool Bin_Heap_contains(Bin_Heap* heap, int index);

#endif