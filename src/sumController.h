#ifndef PROJECT_JIMP2_SUMCONTROLLER_H
#define PROJECT_JIMP2_SUMCONTROLLER_H
#include "tree.h"
typedef struct{
    char *binaryRepresentation;
}controlSums_t;

controlSums_t * getControlSums(int compressionRatio,frequency_t *frequency, codes_t *codes, int size, int restBits);
#endif //PROJECT_JIMP2_SUMCONTROLLER_H
