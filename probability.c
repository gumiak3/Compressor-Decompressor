#include "probability.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int compare_by_word(const void *a, const void *b){
    return (*(char*)a - *(char*)b);
}
int contains(probability_t * array, char *word){
    for(int i=0;i<11;i++){
        if(array[i].word)
            if(compare_by_word(array[i].word, word) == 0){
                return i;
            }
    }
    return -1;
}
int compare_by_amount(const void* a, const void* b) {
    const probability_t* p_a = (const probability_t*) a;
    const probability_t* p_b = (const probability_t*) b;
    return p_b->amount - p_a->amount;
}
probability_t * readBinaryFile(char *fileName, int flag){
    FILE *in = fopen(fileName,"rb");
    unsigned char buffer;
    probability_t *probability = malloc(sizeof(*probability) * 127);
    int index = 0;
    if(in!=NULL){
        while(fread(&buffer,sizeof(unsigned char),2,in) == flag){
            int temp;
            if((temp = contains(probability,&buffer)) >=0){
                probability[temp].amount++;
            }else {
                probability[index].word = malloc(sizeof(char));
                strcpy(probability[index].word, &buffer);
                probability[index].amount = 1;
                index++;
            }
        }
    }else{
        printf("Couldn't open the file!\n");
    }
    sortArray(probability);
    fclose(in);
    return probability;
}

void sortArray(probability_t *array){
    qsort(array,127,sizeof(probability_t),compare_by_amount);
}

void free_memory(probability_t *array){
    free(array);
}