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
    int dekompres = 0;
    // UWAZAJ U CIEBIE MOGA BYC INNE SCIEZKI DO PLIKOW
    // JAK CHCESZ SKOMPRESOWAC TO:
    // - ZMIENIASZ PLIK W 15 LINJCE NA TEN CO CHCESZ SKOMENTOWAC
    // - KOMENTUJESZ OD 23 LINIJKI DO 38 (WLACZNIE).
    // JAK CHCESZ DEKOMPRESOWAC TO:
    // - ZMIENIASZ PLIK W 15 LINIJCE NA TEN CO CHCESZ ZDEKOMPESOWAC W NASZYM PRZYPADKU BEDZIE TO output.txt (no chyba, ze zmieniles w pliku compress.c (tam jest mozliwa zmiana)
    // - KOMENTUJESZ OD 39 DO 46
    if(dekompres){
        char *data = readData("output.txt", &size);
        int *controlSums;
        if (headerCheck(data, &size)) {
            controlSums = getCompressSums(data, &size);
        }
        if (controlSums[2] != 0) {
            size--; // jeżeli zostaje reszta, którą nie kompresujemy
        }
        int dictionarySize = 0;

        Output *dictionary = getDictionary(data, &size, compressionRatio, &dictionarySize);
        for (int i = 0; i < dictionarySize; i++) {
            printf("%c -> %s\n", dictionary[i].bits, dictionary[i].code);
        }

        char *finalData = getBitsInChar(data, &size, controlSums[1]);
        decoder(dictionary, finalData, dictionarySize, compressionRatio);
        }
    else{
        char *data = readData("../Compressor-Decompressor/t.jpg", &size);
        char rest = 0; // reszta po podzieleniu na 12 lub 16
        int restBits; // ilosc bitow ile zajmuje reszta
        short *splittedData = splitData(data, &size, compressionRatio, &rest, &restBits);
        int dataSize = size;
        frequency_t *freqArray = getFrequency(splittedData, &size);
        Output *codes = get_codes(freqArray, size);
        controlSums_t * controlSums = getControlSums(compressionRatio, freqArray, codes, size, restBits);
        compressFile(splittedData,dataSize,codes,size,&rest, controlSums,compressionRatio);
    }
    return 0;
}