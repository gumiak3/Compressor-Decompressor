#include "compress.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
short convertToBits(char *code, int length){
    short output = 0x0;
    for(int i=0;i<length;i++){
        if(code[i]=='1'){
            output |= (1 << (length- 1 - i));
        }
    }
    return output;
}
char *intToBinary(int number, int length){
    char *output = malloc(sizeof(*output) * length + 1);
    int i = 0;
    if (number < 0) {
        number = -number;
        number = ((1 << (length-1)) - number);
        output[i++] = '1';
    } else {
        output[i++] = '0';
    }

    for (int j = length-2; j >= 0; j--) {
        if (number & (1 << j)) {
            output[i++] = '1';
        } else {
            output[i++] = '0';
        }
    }

    output[i] = '\0';

    return output;
}


char * getCode(short bits, codes_t *codes, int size){
    for(int i=0;i<size;i++){
        if(bits==codes[i].bits){
            return codes[i].code;
        }
    }
    return NULL;
}


void writeHeadline(char * headline, FILE *out){
    fwrite(headline,sizeof(*headline),strlen(headline),out);
}


void writeControlSumToFile(controlSums_t * controlSums, FILE *out){
    char buffor2[8] = {};
    for(int i=0;i<3;i++){
        for(int j=0;j<8-strlen(controlSums[i].binaryRepresentation);j++){
            buffor2[j] = '0';
        }
        for(int j=8-strlen(controlSums[i].binaryRepresentation);j<8;j++){
            buffor2[j] = controlSums[i].binaryRepresentation[j - (8-strlen(controlSums[i].binaryRepresentation))];
        }
        char toWrite = convertToBits(buffor2,8);
        fwrite(&toWrite,sizeof(char),1,out);
    }
    char toWrite = 0;
    fwrite(&toWrite,sizeof(char),1,out);
}

char* toChar(short bits, int length){
    char *buffor = malloc(sizeof(*buffor) * (length+1));
    memset(buffor,'0',length);
    buffor[length] = '\0';
    for(int i = length-1;i>=0;i--){
        if(bits & (1 << i)){
            buffor[length- 1- i] = '1';
        }
    }
    return buffor;
}
void writeDictionarySizeToFile(int size,FILE *out){
    short toWrite = size;
    toWrite = ((toWrite & 0xFF00) >> 8) | ((toWrite & 0x00FF) << 8);
    fwrite(&toWrite,2,1,out);
}
void writeDictionary(codes_t *codes, int size, FILE *out,int compressionRatio){
    writeDictionarySizeToFile(size,out);
    char *code = toChar(codes[0].bits,compressionRatio);
    int codeIndex=0;
    char buffor[8];
    int bufforIndex =0;
    int i=0;
    int j=0; // helper
    while(i<size){
        while(bufforIndex < 8 && codeIndex < strlen(code)){
            buffor[(bufforIndex)++] = code[codeIndex++];
        }
        if(bufforIndex == 8){
            char toWrite = convertToBits(buffor, 8);
            fwrite(&toWrite,sizeof(char),1,out);
            bufforIndex = 0;
        }
        if(codeIndex == strlen(code)){
            if(j%3==2) {
                i++;
            }
            switch(j%3){
                case 0:{
                    free(code);
                    code = intToBinary(strlen(codes[i].code),8);
                    break;
                }
                case 1:{
                    free(code);
                    code = codes[i].code;
                    break;
                }
                case 2:{
                    code = toChar(codes[i].bits,compressionRatio);
                    break;
                }
            }
            j++;
            codeIndex = 0;
        }
    }
    if(bufforIndex!=0){
        for(int j=7;j>=bufforIndex;j--){
            buffor[j]='0';
        }
        char toWrite = convertToBits(buffor, 8);
        fwrite(&toWrite,sizeof(char),1,out);
    }
    free(code);

}
int string_length(char *str) {
    if (str == NULL) {
        return 0; // handle null pointer case
    }
    int length = 0;
    while (*str != '\0') {
        length++;
        str++;
    }
    return length;
}
void compressFile(short *splittedData,int dataSize, codes_t *codes, int codesSize, char *rest, controlSums_t *controlSums, int compressionRatio, FILE *out){
    writeHeadline("#LP#",out);
    writeControlSumToFile(controlSums,out);
    int codeLength=0;
    int codeIndex=0;
    int bufforIndex = 0;
    char buffor[8] = {};
    int i=0;
    writeDictionary(codes,codesSize,out,compressionRatio);
    char *code = getCode(splittedData[i++],codes,codesSize); // not freed
    codeLength = string_length(code);
    char control = 0;
    while(i <= dataSize){
        while(bufforIndex < 8  && codeIndex < codeLength){
            buffor[bufforIndex++] = code[codeIndex++];
        }
        if(bufforIndex == 8){
            char toWrite = convertToBits(buffor, 8);
            control^=toWrite;
            fwrite(&toWrite,sizeof(char),1,out);
            bufforIndex = 0;
        }
        if(codeIndex == codeLength){
            code = getCode(splittedData[i++],codes,codesSize);
            codeLength = string_length(code);
            codeIndex = 0;
        }
    }
    if(bufforIndex!=0){
        // zostala jakas reszta nie zapisana
        for(int j=7;j>=bufforIndex;j--){
            buffor[j]='0';
        }
        char toWrite = convertToBits(buffor, 8);
        control^=toWrite;
        fwrite(&toWrite,sizeof(char),1,out);
    }
    // nastepna reszta ktora dostalismy ze splitowania danych
    if(*rest != 0){
        control^=*rest;
        fwrite(rest,sizeof(char),1,out);
    }
    fseek(out,7,SEEK_SET);
    fwrite(&control,sizeof(char),1,out);
    fclose(out);
}

