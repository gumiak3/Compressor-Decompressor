#include "compress.h"
#include <stdio.h>
#include <string.h>
#include "frequency.h"
char convertToBits(char *code, int length){
    char output = 0x0;
    for(int i=0;i<length;i++){
        if(code[i]=='1'){
            output |= (1 << (length- 1 - i));
        }
    }
    return output;
}

char * getCode(short bits, codes_t *codes, int size, int *codeLength){
    for(int i=0;i<size;i++){
        if(bits==codes[i].bits){
            *codeLength = strlen(codes[i].code);
            return codes[i].code;
        }
    }
    return NULL;
}
//
//void addControlSums(char *rest,)

void compressFile(short *splittedData,int dataSize, codes_t *codes, int codesSize, char *rest){
    FILE *out = fopen("output.txt", "wb");
    char *headline = "#LP#\n";
    fwrite(headline,sizeof(*headline),strlen(headline),out);
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
        for(int i=7;i>=bufforIndex;i--){
            buffor[i]='0';
        }
        char toWrite = convertToBits(buffor, 8);
        fwrite(&toWrite,sizeof(char),1,out);
        // dodac jakas sume kontrolna, ktora powie ze na koncu jest jakas reszta
    }
    // add to the end the rest which we had at the beginning
    fclose(out);
}