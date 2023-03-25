#include <stdio.h>
#include "dataReader.h"
#include "splitData.h"
#include "frequency.h"
#include "tree.h"
#include "compress.h"
#include <string.h>
#include "sumController.h"
int main(int argc, char**argv) {
    int size = 0;
    int compressionRatio = 8;
    char *data = readData(argv[1], &size);
    char rest; // reszta po podzieleniu na 12 lub 16
    int restBits; // ilosc bitow ile zajmuje reszta
    short *splittedData = splitData(data, &size, compressionRatio, &rest, &restBits);
    int dataSize = size;
    frequency_t *freqArray = getFrequency(splittedData, &size);
    Output *codes = get_codes(freqArray, size);
    FILE *testFile = fopen("codes.txt", "w");
    for (int i = 0; i < size; i++) {
        fputc(codes[i].bits, testFile);
        fputc(' ', testFile);
        for (int j = 0; j < strlen(codes[i].code); j++) {
            fputc(codes[i].code[j], testFile);
        }
        fputc('\n', testFile);
    }
    controlSums_t * controlSums = getControlSums(compressionRatio, freqArray, codes, size, restBits);
    compressFile(splittedData,dataSize,codes,size,&rest, controlSums,compressionRatio);
    return 0;
}