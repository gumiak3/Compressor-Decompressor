#include "stripCompressFile.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

void removeItems(unsigned char *data, int *size, int amountToRemove){
    memmove(data,data+amountToRemove,*size-amountToRemove);
    *size-=amountToRemove;
}
int headerCheck(unsigned char *data, int *size) {
    char *header = "#LP#";
    for(int i=0;i<4;i++){
        if(data[i]!=header[i])
            return 0;
    }
    removeItems(data,size,4);
    return 1;
}


int *getCompressSums(unsigned char *data, int *size){
    int *sums = malloc(sizeof(int) *3);
    for(int i=0;i<3;i++){
        sums[i] = data[i];
    }
    removeItems(data,size,3);
    return sums;
}

// getDictionary()
void concatenate_strings(char* a, char* b, char* result) {
    strcpy(result, a);
    strcat(result, b);
}
int binary_to_decimal(char* binary) {
    int length = strlen(binary);
    int decimal = 0;
    for (int i = 0; i < length; i++) {
        int bit = binary[length - i - 1] - '0';
        decimal += bit * pow(2, i);
    }

    return decimal;
}

int getDictionarySize(char *a, char *b){
    char * result = malloc(sizeof(char)*2);
    concatenate_strings(a,b,result);
    int size = binary_to_decimal(result);
    free(result);
    return size;
}

Output * getDictionary(unsigned char *data, int *size, int compressionRatio, int *dictionarySize){
    *dictionarySize = getDictionarySize(intToBinary(data[0],8), intToBinary(data[1],8));
    removeItems(data,size,2);
    Output *dictionary = malloc(sizeof(*dictionary)* (*dictionarySize));
    // posiadamy tablicę z elementami słownika oraz danymi z pliku oraz resztami
    int i=0;
    int bufforIndex = 0;
    int tempIndex = 0;
    int codeIndex = 0;
    int codeLength = compressionRatio;
    int dataIndex = 0;
    char *temp = intToBinary(data[dataIndex++],8);
    int whichData = 0;
    char *buffor = malloc(sizeof(char)*16);
    int amountToRemove = 1;
    while(i < *dictionarySize){
        // biore 8 bitow z data
        // biore compressionRatio bitów
        while(tempIndex < 8 && codeIndex < codeLength){
            buffor[bufforIndex++] = temp[tempIndex++];
            codeIndex++;
        }
        if(tempIndex == 8){
            tempIndex = 0;
            if (dataIndex > *size) {
                printf("Error: dataIndex exceeds data size\n");
                exit(1);
            }
            temp = intToBinary(data[dataIndex++],8);
            amountToRemove++;
        }
        if(codeIndex==codeLength){
            buffor[bufforIndex] = '\0';
            switch(whichData%3){
                case 0:{
                    dictionary[i].bits = binary_to_decimal(buffor);
                    codeLength = 8;
                    break;
                }
                case 1:{
                    codeLength = binary_to_decimal(buffor);
                    break;
                }
                case 2:{
                    dictionary[i].code = malloc(sizeof(*dictionary[i].code)*codeLength);
                    for(int k=0;k<codeLength;k++){
                        dictionary[i].code[k] = buffor[k];
                    }
                    dictionary[i].code[codeLength] = '\0';
                    codeLength = compressionRatio;
                    i++;
                    break;
                }
            }
            whichData++;
            memset(buffor,0,16);
            bufforIndex=0;
            codeIndex = 0;
        }
    }
    removeItems(data,size,amountToRemove);
    return dictionary;
}

char *getBitsInChar(unsigned char *data, int *size, int rest){
    int numberOfBitsToRead;
    printf("%d\n",rest);
    if(rest>0){
        numberOfBitsToRead = *size * 8 - (8-rest); // cos tu zle
    }else{
        numberOfBitsToRead = *size*8;
    }
    int helper = numberOfBitsToRead;
    char *output = malloc(sizeof(*output)*numberOfBitsToRead);
    int i=0;
    int d = 0;
    int index=0;
    while(i<*size){
        char *temp = intToBinary(data[i++],8);
        int tempIndex =0;
        while(numberOfBitsToRead>0 && tempIndex < 8){
            output[index++] = temp[tempIndex++];
            numberOfBitsToRead--;
        }
    }
    *size = helper;
    output[index] = '\0';
    return output;
}