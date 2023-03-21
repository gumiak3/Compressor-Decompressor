#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "dataReader.h"
#include "splitData.h"
#include "frequency.h"
#include "compress.h"
#include "tree.h"


int main(int argc, char**argv) {
    int size = 0;
    char *data = readData(argv[1],&size);
    char rest;
    short *splittedData = splitData(data,&size,8,&rest);
    int dataSize = size;
    frequency_t * freqArray = getFrequency(splittedData,&size);
    Output *codes = get_codes(freqArray, size);
//    printf("rozmiar size to: %d", size);
//    for(int i = 0; i < size; i++){
//        printf("Bitsy to: %c, a ich kod to: %s \n",  codes[i].bits, codes[i].code);
//    }

    return 0;

}
