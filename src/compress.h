//
// Created by Lukasz on 19.03.2023.
//

#ifndef COMPRESSOR_COMPRESS_H
#define COMPRESSOR_COMPRESS_H
#include "sumController.h"
#include <stdio.h>
void compressFile(short *splittedData,int dataSize, codes_t *codes, int codesSize, char *rest, controlSums_t *controlSums, int compressionRation, FILE *out);
char * intToBinary(int number, int length);
short convertToBits(char *code, int length);
#endif //COMPRESSOR_COMPRESS_H
