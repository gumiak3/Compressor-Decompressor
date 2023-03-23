#include "compress.h"
#include <stdio.h>
#include <string.h>
char convertToBits(char *code, int length){
    char output = 0x0;
    for(int i=0;i<length;i++){
        if(code[i]=='1'){
            output |= (1 << (length- 1 - i));
        }
    }
    return output;
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
        printf("%s\n",buffor2);
        char toWrite = convertToBits(buffor2,8);
        fwrite(&toWrite,sizeof(char),1,out);
    }
    fwrite("\n",sizeof(char),1,out);
}

void compressFile(short *splittedData,int dataSize, Output *codes, int codesSize, char *rest, controlSums_t *controlSums){
    FILE *out = fopen("output.bin", "wb");
    writeHeadline("#LP#",out);
    writeControlSumToFile(controlSums,out);
    int codeLength=0;
    int codeIndex=0;
    int bufforIndex = 0;
    char buffor[8] = {};
    int i=0;
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
    if(bufforIndex!=0){
        // zostala jakas reszta nie zapisana
        for(int j=7;j>=bufforIndex;j--){
            buffor[j]='0';
        }
        char toWrite = convertToBits(buffor, 8);
        fwrite(&toWrite,sizeof(char),1,out);
    }
    // nastepna reszta ktora dostalismy ze splitowania danych
    if(*rest != 0)
        fwrite(rest,sizeof(char),1,out);
    fclose(out);
}

