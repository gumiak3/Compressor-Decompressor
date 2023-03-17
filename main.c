#include <stdio.h>
#include <stdlib.h>
#include "dataReader.h"
#include "splitData.h"
#include "frequency.h"
int main(int argc, char**argv) {
    int size = 0;
    char *data = readData(argv[1],&size);
    char rest;
    short *splittedData = splitData(data,&size,8,&rest);
    printf("size %d\n", size);
    frequency_t * freqArray = getFrequency(splittedData,&size);
    printf("size %d\n", size);
    for(int i=0;i<size;i++){
        printf("%c -> %d\n",freqArray[i].bits, freqArray[i].frequency);
    }
    return 0;

}
