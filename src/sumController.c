#include "sumController.h"
#include "string.h"
#include <stdlib.h>
#include <stdio.h>
int getCodeLength(short bits, codes_t *codes, int size, int *codeLength){
    for(int i=0;i<size;i++){
        if(bits==codes[i].bits){
            return strlen(codes[i].code);
        }
    }
    return 0;
}
char * reverseArray(char *array){
    char *temp = malloc(sizeof(*temp) * 8);
    int j=0;
    temp[strlen(array)] = '\0';
    for(int i=strlen(array)-1;i>=0;i--){
        temp[i] = array[j++];
    }
    free(array);
    return temp;
}
char * toBinary(int number){
    char *output = malloc(sizeof(*output) * 8);
    int i = 0;
    while (number > 0) {
        output[i++] = (number % 2) + '0';
        number /= 2;
    }
    output[i] = '\0';
    return reverseArray(output); // reversing the array
}

int getControlSumOfCodeRest(frequency_t *frequency, codes_t *codes, int size,int numberOfBitsToRead){ // liczy ile bitow czytamy z reszty, która wypada podczas wpisywania bitow do pliku
    int sumBits = 0;
    int temp = 0;
    for(int i=0;i<size;i++){
        sumBits+=frequency[i].frequency * getCodeLength(frequency[i].bits,codes,size,&temp);
    }
    return sumBits%8;
}


controlSums_t *getControlSums(int compressionRatio,frequency_t *frequency, codes_t *codes, int size, int restBits){

    controlSums_t *sums = malloc(sizeof(*sums) * 3);
    sums[0].binaryRepresentation = toBinary(compressionRatio);
    sums[1].binaryRepresentation = toBinary(getControlSumOfCodeRest(frequency,codes,size,compressionRatio));
    sums[2].binaryRepresentation = toBinary(restBits);
    free(frequency);
    return sums;
}

void freeControlSums(controlSums_t *sums){
    for(int i=0;i<3;i++){
        free(sums[i].binaryRepresentation);
    }
    free(sums);
}


