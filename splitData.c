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
short *splitTo12(unsigned char *data, int *size,int *newSize,char *rest, int *restBits){
    *newSize = ((*size) * 8 )/12;
    short *outputData = malloc(sizeof(*outputData) * (*newSize));
    int index = 0;
    int numberOfRest = (((*size) * 8 )%12);
    int length;
    if(numberOfRest == 8){
        // 8 bitow zostaje
        length = *size-1;
    }else{
        length = *size;
    }
    for (int i = 0; i < length; i += 3) {
        outputData[index] = (data[i]);
        outputData[index] = (outputData[index] << 4) | ((data[i + 1] >> 4) & 0x00FF);
        index++;
        if((i + 2) < length){
            outputData[index] = (data[i+1] << 4) & 0x00FF ;
            outputData[index] = (outputData[index] << 4) | (data[i + 2]);
        }else{
            *rest = (data[i+1]<<4);
        }
        index++;
    }
    if(numberOfRest==8)
        *rest = data[length];
    *restBits = numberOfRest;
    return outputData;
}
short *splitTo16(unsigned char *data, int *size, int *newSize,unsigned char *rest, int *restBits){
    int mask = 0x00FF; // to extract useless 1
    *newSize = ((*size) * 8)/16;
    short *outputData = malloc(sizeof(*outputData) * *newSize);
    int index = 0;
    int length = *size%2==0 ? *size : *size-1;
    for(int i=0;i<length;i+=2){
        outputData[index] = data[i];
        outputData[index] = (outputData[index] <<  8);
        outputData[index] |= data[i+1];
        index++;
    }
    if((*size)%2!=0){
        *rest = data[*size-1];
        *restBits = 8;
        // we got rest of bits;
    }else{
        *restBits = 0;
    }
    return outputData;
}

short *splitData(unsigned char *data, int *size,int bitsToRead, unsigned char *rest, int *restBits){
    short *splittedData;
    int newSize = 0;
    switch(bitsToRead){
        case 8:{
            splittedData = splitTo8(data,size,rest);
            *restBits = 0;
            break;
        }
        case 12:{
            splittedData = splitTo12(data,size,&newSize,rest,restBits);
            *size = newSize;
            break;
        }
        case 16:{
            splittedData = splitTo16(data,size,&newSize,rest,restBits);
            *size = newSize;
            break;
        }
    }
    return splittedData;
}