#ifndef COMPRESSOR_DECOMPRESSOR_TREE_H
#define COMPRESSOR_DECOMPRESSOR_TREE_H
#include "frequency.h"
typedef struct codes_t {
    short bits;
    char *code;
} codes_t;

codes_t * get_codes(frequency_t  *freqArray, int n);

#endif //COMPRESSOR_DECOMPRESSOR_TREE_H
