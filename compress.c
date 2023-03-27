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
char * intToBinary(int number, int length){
    char *output = malloc(sizeof(*output) * length + 1);
    int i = 0;
    while (number > 0) {
        output[i++] = number%2==0 ? '0' : '1';
        number /= 2;
    }
    output[i] = '\0';
    for(int j=i;j<length;j++){
        output[j] = '0';
    }
    output[length] = '\0';

    return strrev(output); // reversing the array
}

char * getCode(short bits, Output *codes, int size, int *codeLength){
    for(int i=0;i<size;i++){
        if(bits==codes[i].bits){
            *codeLength = strlen(codes[i].code);
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
    short toWrite = convertToBits(intToBinary(size,16),16);
    toWrite = ((toWrite & 0xFF00) >> 8) | ((toWrite & 0x00FF) << 8);
    fwrite(&toWrite,2,1,out);
}
void writeDictionary(Output *codes, int size, FILE *out,int compressionRatio){
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
            // wpisujemy do pliku
            char toWrite = convertToBits(buffor, 8);
            fwrite(&toWrite,sizeof(char),1,out);
            bufforIndex = 0;
        }
        if(codeIndex == strlen(code)){
            if(j%3==2)
                i++;
            switch(j%3){
                case 0:{
                    code = intToBinary(strlen(codes[i].code),8);
                    break;
                }
                case 1:{
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
}

void compressFile(short *splittedData,int dataSize, Output *codes, int codesSize, char *rest, controlSums_t *controlSums, int compressionRatio){
    printf("%d\n", convertToBits(intToBinary(12345,16),16));
    FILE *out = fopen("../Compressor-Decompressor/output.bin", "wb");
    writeHeadline("#LP#",out);
    writeControlSumToFile(controlSums,out);
    int codeLength=0;
    int codeIndex=0;
    int bufforIndex = 0;
    char buffor[8] = {};
    int i=0;
    writeDictionary(codes,codesSize,out,compressionRatio);
    char *code = getCode(splittedData[i++],codes,codesSize,&codeLength);
    while(i <= dataSize){
        while(bufforIndex < 8  && codeIndex < codeLength){
            buffor[bufforIndex++] = code[codeIndex++];
        }
        if(bufforIndex == 8){
            // wpisujemy do pliku
            char toWrite = convertToBits(buffor, 8);
            fwrite(&toWrite,sizeof(char),1,out);
            bufforIndex = 0;
        }
        if(codeIndex == codeLength){
            // bierzemy nowy kod
            code = getCode(splittedData[i++],codes,codesSize,&codeLength);
            codeIndex = 0;
        }
    }
    printf("%s\n",buffor);
    if(bufforIndex!=0){
        // zostala jakas reszta nie zapisana
        for(int j=7;j>=bufforIndex;j--){
            buffor[j]='0';
        }
        char toWrite = convertToBits(buffor, 8);
        fwrite(&toWrite,sizeof(char),1,out);
    }
    printf("%d\n", *rest);
    // nastepna reszta ktora dostalismy ze splitowania danych
    if(*rest != 0)
        fwrite(rest,sizeof(char),1,out);
    fclose(out);
}

