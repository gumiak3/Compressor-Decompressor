
#include <stdbool.h>
#include "dataReader.h"
#include "splitData.h"
#include "frequency.h"
#include "tree.h"
#include "compress.h"


int main(int argc, char**argv) {
    int size = 0;
    char *data = readData(argv[1],&size);
    char rest;
    short *splittedData = splitData(data,&size,8,&rest);
    int dataSize = size;
    frequency_t * freqArray = getFrequency(splittedData,&size);
    Output *codes = get_codes(freqArray, size);
    compressFile(splittedData,dataSize,codes,size,&rest);
    return 0;
}
