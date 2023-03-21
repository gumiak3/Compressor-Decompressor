#include "frequency.h"
#include <stdlib.h>
#include <stdio.h>

void clearArray(frequency_t *array, int size){
    for(int i=0;i<size;i++){
        array[i].bits = 0;
        array[i].frequency = 0;
    }
}

int contains(frequency_t * array, short newElement, int size){
    for(int i=0;i<size;i++){
        if(array[i].bits)
            if(array[i].bits == newElement)
                return i;
    }
    return -1;
}

frequency_t * getFrequency(short *data, int *size){
    frequency_t * frequencyArray = malloc(sizeof(*frequencyArray) * (*size));
    clearArray(frequencyArray,*size);
    int index=0;
    for(int i=0;i<*size;i++){
        if( contains(frequencyArray,data[i],*size) != (-1)){
            frequencyArray[contains(frequencyArray,data[i],*size)].frequency++;
        }else{
            frequencyArray[index].bits = data[i];
            frequencyArray[index].frequency = 1;
            index++;
        }
    }
    *size = index;
    return frequencyArray;
}

