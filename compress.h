//
// Created by Lukasz on 19.03.2023.
//

#ifndef COMPRESSOR_COMPRESS_H
#define COMPRESSOR_COMPRESS_H
#include "sumController.h"
void compressFile(short *splittedData,int dataSize, Output *codes, int codesSize, char *rest, controlSums_t *controlSums, int compressionRation);
char * intToBinary(int number, int length);
#endif //COMPRESSOR_COMPRESS_H
