#include "probability.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int contains(probability_t * array, char *word){
    for(int i=0;i<11;i++){
        if(array[i].word)
            if(strcmp(array[i].word,word) == 0)
                return 1;
    }
    return 0;
}

probability_t * readBinaryFile(char *fileName, int flag){
    FILE *in = fopen(fileName,"rb");
    unsigned char buffer[flag];
    probability_t *probability = malloc(sizeof(*probability) * 127);
    int index = 0;
    if(in!=NULL){
        while(fread(buffer,sizeof(unsigned char),flag,in) == flag){
            if(contains(probability,buffer)){
                probability[index].amount++;
            }else{
                probability[index].word = malloc(sizeof(char));
                strcpy(probability[index].word, buffer);
                probability[index].amount = 1;
            }
            index++;
        }
    }else{
        printf("Couldn't open the file!\n");
    }
    return probability;
}
// get bit by bit
//    for (int i = 0; i < 1; i++) {
//        for (int j = 7; j >= 0; j--) {
//            printf("%d", (probability[i] >> j) & 1);
//        }
//        printf(" ");
//    }

void free_memory(probability_t *array){
    free(array);
}