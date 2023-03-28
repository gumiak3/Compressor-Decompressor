#include <stdio.h>
#include "dataReader.h"
#include "splitData.h"
#include "frequency.h"
#include "tree.h"
#include "compress.h"
#include <string.h>
#include "sumController.h"
#include "stripCompressFile.h"
#include "decompressor.h"

int main(int argc, char**argv) {
    int size = 0;
    int compressionRatio = 8;
    unsigned char *data = readData("../Compressor-Decompressor/output.txt", &size);

    int *controlSums;
    if(headerCheck(data,&size)){
        controlSums = getCompressSums(data,&size);
    }
    if(controlSums[2]!=0){
        size--; // jeżeli zostaje reszta, którą nie kompresujemy
    }
    int dictionarySize = 0;
    Output *dictionary = getDictionary(data,&size,compressionRatio,&dictionarySize);
    for(int i=0;i<dictionarySize;i++){
        printf("%c -> %s\n",dictionary[i].bits, dictionary[i].code);
    }
    char *finalData = getBitsInChar(data,&size,controlSums[1]);
    printf("%s\n",finalData);
    decoder(dictionary,finalData,dictionarySize,compressionRatio);
//
//    unsigned char rest = 0; // reszta po podzieleniu na 12 lub 16
//    int restBits; // ilosc bitow ile zajmuje reszta
//    short *splittedData = splitData(data, &size, compressionRatio, &rest, &restBits);
//    int dataSize = size;
//    frequency_t *freqArray = getFrequency(splittedData, &size);
//    Output *codes = get_codes(freqArray, size);
//    for(int i=0;i<size;i++){
//        printf("%d -> code: %s\n", codes[i].bits , codes[i].code);
//    }
//    FILE *testFile = fopen("../Compressor-Decompressor/codes.txt", "w");
//    for (int i = 0; i < size; i++) {
//        fputc(codes[i].bits, testFile);
//        fputc(' ', testFile);
//        for (int j = 0; j < strlen(codes[i].code); j++) {
//            fputc(codes[i].code[j], testFile);
//        }
//        fputc('\n', testFile);
//    }
//    controlSums_t * controlSums = getControlSums(compressionRatio, freqArray, codes, size, restBits);
//    compressFile(splittedData,dataSize,codes,size,&rest, controlSums,compressionRatio);
    return 0;
}