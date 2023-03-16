#include "splitData.h"
#include <stdlib.h>
short *splitTo8(char *data, int *size, short *rest){
    short *outputData = malloc(sizeof(*outputData) * *size);
    for(int i=0;i<*size;i++){
        outputData[i] = data[i];
    }
    return outputData;
}
short *splitTo12(char *data, int *size,int *newSize, short *rest){
    *newSize = ((*size) * 8 )/12;
    short *outputData = malloc(sizeof(*outputData) * *newSize);
    int index = 0;
    short temp;
    for(int i=0;i<*size;i++){

    }
    return NULL;

}
short *splitTo16(char *data, int *size, int *newSize, short *rest){
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

short *splitData(char *data, int *size,int bitsToRead, short *rest){
    short *splittedData;
    int newSize = 0;
    switch(bitsToRead){
        case 8:{
            splittedData = splitTo8(data,size,rest);
            break;
        }
        case 12:{
            splittedData = splitTo12(data,size,&newSize,rest);
            break;
        }
        case 16:{
            splittedData = splitTo16(data,size,&newSize,rest);
            break;
        }
    }
    *size = newSize;
    return splittedData;
}