//
// Created by pawel on 20/03/2023.
//

#ifndef COMPRESSOR_DECOMPRESSOR_TREE_H
#define COMPRESSOR_DECOMPRESSOR_TREE_H

typedef struct Output {
    short bits;
    char *code;
} Output;

Output * get_codes( struct frequency_t  *freqArray, int n);

#endif //COMPRESSOR_DECOMPRESSOR_TREE_H
