//
// Created by Lukasz on 26.03.2023.
//

#ifndef PROJECT_JIMP2_STRIPCOMPRESSFILE_H
#define PROJECT_JIMP2_STRIPCOMPRESSFILE_H
#include "tree.h"
#include "sumController.h"
#include "compress.h"
int headerCheck(unsigned char *data, int *size);
int *getCompressSums(unsigned char *data, int *size);
Output *getDictionary(unsigned char *data, int *size, int compressionRatio,int *dictionarySize);
char *getBitsInChar(unsigned char *data, int *size, int rest);
#endif //PROJECT_JIMP2_STRIPCOMPRESSFILE_H
