//
// Created by Lukasz on 19.03.2023.
//

#ifndef COMPRESSOR_COMPRESS_H
#define COMPRESSOR_COMPRESS_H
#include "tree.h"
void compressFile(short *splittedData,int dataSize, Output *codes, int codesSize, char *rest);
#endif //COMPRESSOR_COMPRESS_H
