//
// Created by Lukasz on 16.03.2023.
//
#include <stdio.h>
#include <stdlib.h>
#include "dataReader.h"
void doubleSize(char *array, int *length){
    *length*=2;
    array = realloc(array, sizeof(*array) * *length);
}

char * readData(char *fileName, int *size){
    int length = 1024;
    FILE *in = fopen(fileName,"rb");
    if(in == NULL){
        fprintf(stderr,"Couldn't open the file!\n");
        return NULL;
    }
    char ch;
    char *data = malloc(sizeof(*data) * length);
    while((ch = fgetc(in))!=EOF){
        data[*size] = ch;
        (*size)++;
        if(*size == length)
            doubleSize(data,&length);
    }
    fclose(in);
    return data;
}
