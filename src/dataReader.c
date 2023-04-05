//
// Created by Lukasz on 16.03.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include "dataReader.h"
void doubleSize(char *data, int *length){
    *length*=2;
    data = realloc(data, sizeof(*data) * *length);
}
// czy jest to plik kompressowany albo czy dostalismy flage


unsigned char * readData(char *fileName, int *size){
    int length = 1024;
    FILE *in = fopen(fileName,"rb");
    if(in == NULL){
        fprintf(stderr,"Couldn't open the file!\n");
        return NULL;
    }
    unsigned char *data = malloc(sizeof(*data) * length);
    unsigned char buffor;
    while(fread(&buffor,1,sizeof(buffor),in) != 0){
        data[*size] = buffor;
        (*size)++;
        if(*size == length){
            length*=2;
            data = realloc(data, sizeof(*data) * length);
        }
    }
    fclose(in);
    return data;
}

