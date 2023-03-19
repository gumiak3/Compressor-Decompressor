#include "compress.h"
#include <stdio.h>
#include <string.h>
char convertToBits(char *code){
    char output = 0x0;
    for(int i=0;i<strlen(code);i++){
        if(code[i]=='1'){
            output |= (1 << i);
        }
    }
    return output;
}

void compressFile(short *splittedData,int dataSize, codes_t *codes, int codesSize){
    FILE *out = fopen("output.txt", "wb");
    char *headline = "#LGRP#";
    fwrite(headline,sizeof(*headline),strlen(headline),out);
    int i=8;
}