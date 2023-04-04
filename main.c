#include <stdio.h>
#include <getopt.h>

#include "dataReader.h"
#include "splitData.h"
#include "frequency.h"
#include "tree.h"
#include "compress.h"
#include "sumController.h"
#include "stripCompressFile.h"
#include "decompressor.h"

int main(int argc, char**argv) {

    int size = 0;

//    UWAGA!!!
//    WARTOŚCI DO ZMIENNYCH compressionRatio I dekompres MOŻESZ ZMIENIĆ W LINIJCE 123 I 124
//    INICJALIZUJE JE TUTAJ, ŻEBY FLAGI (JEŻELI SĄ) NADPISAŁY TE WARTOŚCI, BO INACZEJ NIE SKOMPILUJE SIĘ
    /*
    int compressionRatio;
    int dekompres;

    int opt;
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

    while((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
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
    if(help_needed) {
        printf("This is file compressor/decompressor\n"
               "<tutaj jakis sposob jak go odpalic>\n"
               "possible flags:\n"
               "--8, --12, --16 - choose version of algorithm to compress\n"
               "--x, --c - choose mode of program (compress or decompress)\n"
               "--v - show info about compress ratio\n"
               "--help - show this help and quit program\n");
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
    }
    else if(compress_mode) {
        dekompres = 0;
    }
    else if(compress_mode + decompress_mode == 0) {
        //tutaj trzeba zrobic jakies czytanie z pliku i sprawdzanie czy jest tam kompres czy dekompres
    }
    */
    int compressionRatio = 8;
    int dekompres = 1;
    if(dekompres){
        unsigned char *data = readData("output.txt", &size);
        int *controlSums;
        if (headerCheck(data, &size)) {
            controlSums = getCompressSums(data, &size);
        }
        char rest2 = 0;
        if (controlSums[2] != 0) {
            rest2 = data[size-1];
            size--; // jeżeli zostaje reszta, którą nie kompresujemy
        }

        int dictionarySize = 0;

        Output *dictionary = getDictionary(data, &size, compressionRatio, &dictionarySize);
        char restToWrite = 0;
        char *finalData = getBitsInChar(data, &size, controlSums[1]);
        decoder(dictionary, finalData, dictionarySize, compressionRatio,&rest2,controlSums[2]);
        }
    else{
        unsigned char *data = readData("test.txt", &size);
        char rest = 0; // reszta po podzieleniu na 12 lub 16
        int restBits; // ilosc bitow ile zajmuje reszta
        short *splittedData = splitData(data, &size, compressionRatio, &rest, &restBits);
        int dataSize = size;
        frequency_t *freqArray = getFrequency(splittedData, &size);
        Output *codes = get_codes(freqArray, size);
        controlSums_t * controlSums = getControlSums(compressionRatio, freqArray, codes, size, restBits);
        compressFile(splittedData,dataSize,codes,size,&rest, controlSums,compressionRatio);
    }
//    if(info_needed){
//        //tutaj trzeba zrobic jakies wyswietlanie info o kompresji/dekompresji, np jakis procent
//    }
    return 0;
}