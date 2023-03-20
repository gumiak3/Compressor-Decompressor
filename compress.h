//
// Created by Lukasz on 19.03.2023.
//

#ifndef COMPRESSOR_COMPRESS_H
#define COMPRESSOR_COMPRESS_H
typedef struct{
    short bits;
    char *code;
}codes_t;
void compressFile(short *splittedData,int dataSize, codes_t *codes, int codesSize, char *rest);
#endif //COMPRESSOR_COMPRESS_H
