#include <stdio.h>
#include <stdlib.h>
#include "dataReader.h"
#include "splitData.h"
#include "frequency.h"
#include "compress.h"

int main(int argc, char**argv) {
    int size = 0;
    char *data = readData(argv[1],&size);
    char rest;
    short *splittedData = splitData(data,&size,12,&rest);
    printf("%d", size);
    int dataSize = size;
    frequency_t * freqArray = getFrequency(splittedData,&size);
    //codes_t * codes = malloc(sizeof(*codes)*size);
//    for(int i=0;i<size;i++){
//        codes[i].bits = freqArray[i].bits;
//    }
  //  codes[0].code = "0001";
    //codes[1].code = "1000";
   // compressFile(splittedData,dataSize,codes,size,&rest);

    return 0;

}
