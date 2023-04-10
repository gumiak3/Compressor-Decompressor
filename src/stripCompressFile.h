//
// Created by Lukasz on 26.03.2023.
//

#ifndef PROJECT_JIMP2_STRIPCOMPRESSFILE_H
#define PROJECT_JIMP2_STRIPCOMPRESSFILE_H
#include "tree.h"
#include "sumController.h"
#include "compress.h"
int headerCheck(char *data, int *size);
int *getCompressSums( char *data, int *size);
codes_t *getDictionary(char *data, int *size, int compressionRatio,int *dictionarySize, int extraInfo);
char *getBitsInChar(char *data, int *size, int rest);
void freeControlSums(controlSums_t *sums);
#endif //PROJECT_JIMP2_STRIPCOMPRESSFILE_H
