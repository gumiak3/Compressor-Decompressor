#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include "dataReader.h"
#include "splitData.h"
#include "frequency.h"
#include "tree.h"
#include "compress.h"
#include "sumController.h"
#include "stripCompressFile.h"
#include "decompressor.h"
#include <stdlib.h>
void freeMemoryInCompressMode(short *splittedData,codes_t *codes,int codesSize,controlSums_t *controlSums){
    free(splittedData);
    freeMemoryCodes(codes,codesSize);
    freeControlSums(controlSums);
}


void compressMode(unsigned char *data,int compressionRatio, int *size, FILE *out){
    char rest = 0; // reszta po podzieleniu na 12 lub 16
    int restBits; // ilosc bitow ile zajmuje reszta
    short *splittedData = splitData(data, size, compressionRatio, &rest, &restBits);
    int dataSize = *size;
    frequency_t *freqArray = getFrequency(splittedData, size);
    codes_t *codes = get_codes(freqArray, *size);
    controlSums_t * controlSums = getControlSums(compressionRatio, freqArray, codes, *size, restBits);
    compressFile(splittedData,dataSize,codes,*size,&rest, controlSums,compressionRatio,out);
    freeMemoryInCompressMode(splittedData,codes,dataSize,controlSums);
}


void decompressMode(unsigned char *data,int *compressionRatio,int *size,FILE *out){
    int *controlSums;
    controlSums = getCompressSums(data, size);
    char rest2 = 0;
    if (controlSums[2] != 0) {
        rest2 = data[(*size)-1];
        (*size)--; // jeżeli zostaje reszta, którą nie kompresujemy
    }
    *compressionRatio = controlSums[0];
    int dictionarySize = 0;
    codes_t *dictionary = getDictionary(data,size, *compressionRatio, &dictionarySize);
    char restToWrite = 0;
    char *finalData = getBitsInChar(data, size, controlSums[1]);
    decoder(dictionary, finalData, dictionarySize, *compressionRatio,&rest2,controlSums[2],out);
}

void printHelp(){
    printf("This is file compressor/decompressor\n"
           "<program infile outfile [flags]>\n"
           "possible flags:\n"
           "--8, --12, --16 - choose version of algorithm to compress\n"
           "--c, --x - choose mode of program (compress or decompress)\n"
           "--v - show info about compress ratio\n"
           "--help - show this help and quit program\n");
}

int main(int argc, char**argv) {

    int size = 0;
    int compressionRatio;
    int opt;
    int dekompres;
    int option_index = 0;
    int version_8 = 0;
    int version_12 = 0;
    int version_16 = 0;
    int help_needed = 0;
    int info_needed = 0;
    int compress_mode = 0;
    int decompress_mode = 0;
    static struct option long_options[] = {
            {"8", no_argument, NULL, 1},
            {"12", no_argument, NULL, 2},
            {"16", no_argument, NULL, 3},
            {"help", no_argument, NULL, 4},
            {"v", no_argument, NULL, 5},
            {"c", no_argument, NULL, 6},
            {"x", no_argument, NULL, 7},
            {0, 0, 0, 0}
    };
    int amountOfFlags = 0;
    while((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
        amountOfFlags++;
        switch(opt) {
            case 1:
                version_8 = 1;
                break;
            case 2:
                version_12 = 1;
                break;
            case 3:
                version_16 = 1;
                break;
            case 4:
                help_needed = 1;
                break;
            case 5:
                info_needed = 1;
                break;
            case 6:
                compress_mode = 1;
                break;
            case 7:
                decompress_mode = 1;
                break;
            default:
                printf("Invalid flag.\n"
                       "to see help use --help flag.\n");
                return 1;
        }
    }
    if((argc-amountOfFlags) == 1){
        printHelp();
        return 0;
    }
    unsigned char *data = readData(argv[1+amountOfFlags], &size);
    if(data==NULL){
        printHelp();
        return 0;
    }
    FILE *out;
    if((argc-amountOfFlags) > 2){
        out = fopen(argv[2+amountOfFlags],"wb");
    }else{
        printHelp();
        return 0;
    }
    if(help_needed) {
        printHelp();
        return 0;
    }
    int versionsum = version_8 + version_12 + version_16;
    if(versionsum > 1) {
        printf("Only one version flag can be used.\n"
               "to see help use --help flag.\n");
        return 1;
    }
    if(compress_mode + decompress_mode > 1) {
        printf("Only one mode flag can be used.\n"
               "to see help use --help flag.\n");
        return 1;
    }
    if(decompress_mode == 1 && versionsum > 0) {
        printf("Version flag is used only in compress mode.\n"
               "to see help use --help flag.\n");
        return 1;
    }
    if(versionsum == 0) {
        version_8 = 1;
    }

    if(version_8) {
        compressionRatio = 8;
    }
    else if(version_12) {
        compressionRatio = 12;
    }
    else if(version_16) {
        compressionRatio = 16;
    }
    if(decompress_mode) {
        dekompres = 1;
        if(headerCheck(data,&size)==0){
            fprintf(stderr,"infile is not compressed, try --help");
            return 0;
        }
	printf("%d\n",compressionRatio);
        decompressMode(data,&compressionRatio,&size,out);
    }
    else if(compress_mode) {
        dekompres = 0;
        compressMode(data,compressionRatio,&size,out);
    }
    else if(compress_mode + decompress_mode == 0) {
        //tutaj trzeba zrobic jakies czytanie z pliku i sprawdzanie czy jest tam kompres czy dekompres
        if(headerCheck(data,&size)==0){
            compressMode(data,compressionRatio,&size,out);
        }else{
            decompressMode(data,&compressionRatio,&size,out);
        }
    }
//    if(info_needed){
//        //tutaj trzeba zrobic jakies wyswietlanie info o kompresji/dekompresji, np jakis procent
//    }
    return 0;
}
