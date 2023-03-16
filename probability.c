#include "probability.h"
#include <stdio.h>
#include <stdlib.h>

int length = 64;

void doubleLength(probability_t *array){
    length*=2;
    array = realloc(array,sizeof(*array) * length);
}

int contains(probability_t * array, short newBits){
    for(int i=0;i<11;i++){
        if(array[i].bits)
            if(array[i].bits == newBits)
                return i;
    }
    return -1;
}

void resetArray(probability_t *array){
    for(int i=0;i<length;i++)
    {
        array[i].bits = 0;
        array[i].amount = 0;
    }
}

int compare_by_amount(const void* a, const void* b) {
    const probability_t* p_a = (const probability_t*) a;
    const probability_t* p_b = (const probability_t*) b;
    return p_a->amount - p_b->amount;
}
// read data
// split : 8, 12, 16
//
probability_t * readBinaryFile(char *fileName, int flag){
    FILE *in = fopen(fileName,"rb");
    int bitsToRead = flag == 12 ? 2 : flag==16 ? 2 : 1;
    unsigned char buffer[2];
    probability_t *probability = malloc(sizeof(*probability) * length);
    resetArray(probability);
    int index = 0;
    int mask = 0x00FF; // to extract useless 1
    if(in!=NULL){
        while(fread(buffer,sizeof(unsigned char),bitsToRead,in) != 0){
            short temp;
            if((index+1) == length)
                doubleLength(probability);
            if(flag==12) {
                if(index==0) {
                    probability[index].bits = buffer[0];
                    temp = (probability[index].bits << 4) | (buffer[0] >> 4);
                    probability[index+1].bits = (buffer[0] << 4) & mask;
                    probability[index+1].amount = 1;
                }else if(index%2==0){
                    temp = (probability[index].bits << 4) | (buffer[1] >> 4);
                    probability[index+1].bits = (buffer[1] << 4) & mask;
                    probability[index+1].amount = 1;
                }else{
                    temp = (probability[index].bits << 4) | (buffer[0] & mask);
                    probability[index+1].bits = (buffer[1] & mask);
                    probability[index+1].amount = 1;
                }
            }else if(flag == 16){
                temp = (buffer[0] << 8) | (buffer[1] & mask);
            }else if(flag==8){
                temp = buffer[0];
            }
            if(contains(probability,temp)!=-1){
                probability[contains(probability,temp)].amount++;
            }else{
                probability[index].bits = temp;
                probability[index].amount = 1;
            }
            index++;
        }
    }else{
        printf("Couldn't open the file!\n");
    }

//    sortArray(probability);
    fclose(in);
    return probability;
}

void sortArray(probability_t *array){
    qsort(array,getSize(array),sizeof(probability_t),compare_by_amount);
}

int getSize(probability_t *array){ // doesn't work on 12 bits yet
    int i=0;
    int size = 0;
    while(array[i++].bits){
        size++;
    }
    return size;
}

void free_memory(probability_t *array){
    free(array);
}


// write to file in good direction
//            unsigned char buff[2];
//            buff[0] = (value >> 8) & 0xFF;
//            buff[1] = value & 0xFF;
//            fwrite(buff,2,1,out);