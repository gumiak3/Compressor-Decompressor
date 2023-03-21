#include "splitData.h"
#include <stdlib.h>
#include <stdio.h>
short *splitTo8(char *data, int *size, char *rest){
    short *outputData = malloc(sizeof(*outputData) * *size);
    for(int i=0;i<*size;i++){
        outputData[i] = data[i];
    }
    return outputData;
}
short *splitTo12(char *data, int *size,int *newSize, char *rest){
    *newSize = ((*size) * 8 )/12;
    short *outputData = malloc(sizeof(*outputData) * (*newSize));
    int index = 0;
    int length = (((*size) * 8 )%12) == 0 ? *size : *size-1;
    for (int i = 0; i < length; i += 2) {
        outputData[index] = (data[i] & 0x0F);
        outputData[index] = ((data[i] << 4) & 0xF0) | ((data[i + 1] >> 4) & 0x0F);
        index++;
        outputData[index] = (data[i+1] << 4);
        outputData[index] = ((data[i + 1] << 4) & 0xF0) | (data[i + 2] & 0x0F);
        i++;
        index++;
    }
    if((((*size) * 8 )%12) != 0){
        *rest = data[length-1];
    }
    return outputData;

}
short *splitTo16(char *data, int *size, int *newSize, char *rest){
    int mask = 0x00FF; // to extract useless 1
    *newSize = ((*size) * 8)/16;
    short *outputData = malloc(sizeof(*outputData) * *newSize);
    int index = 0;
    int length = *size%2==0 ? *size : *size-1;
    for(int i=0;i<length;i+=2){
        outputData[index++] = (data[i] << 8) | (data[i+1] & mask);
    }
    if(*size%2!=0){
        *rest = data[*size-1];
        // we got rest of bits;
    }
    return outputData;
}

short *splitData(char *data, int *size,int bitsToRead, char *rest){
    short *splittedData;
    int newSize = 0;
    switch(bitsToRead){
        case 8:{
            splittedData = splitTo8(data,size,rest);
            break;
        }
        case 12:{
            splittedData = splitTo12(data,size,&newSize,rest);
            *size = newSize;
            break;
        }
        case 16:{
            splittedData = splitTo16(data,size,&newSize,rest);
            *size = newSize;
            break;
        }
    }

    return splittedData;
}