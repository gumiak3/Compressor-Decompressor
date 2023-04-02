#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdint-gcc.h>
#include "decompressor.h"


void write8Bits(short *bits, FILE *out){
    fwrite(bits,1,1,out);
}
void write12Bits(short *bits, FILE *out){
    // still working on it
}
void write16Bits(short *bits, FILE *out){
    short swapped = ((*bits & 0xFF) << 8) | ((*bits >> 8) & 0xFF); // bitwise operations to swap the bits
    fwrite(&swapped,sizeof(short),1,out);
}
void writeToFile(short *bits, FILE *out, int compressionRatio){
    switch(compressionRatio){
        case 8:
            write8Bits(bits,out);
            break;
        case 12:
            write12Bits(bits,out);
            break;
        case 16:
            write16Bits(bits,out);
            break;
    }
}

typedef struct decode_t{
    short bits;
    int decimal_code;
    char *binary_code;
} decode_t;

typedef struct decode_t2{
    short bits;
    int decimal_code;
    bool exist;
}decode_t2;

int decimal_code_maker(decode_t decimal_codes){
    int length = strlen(decimal_codes.binary_code);
    char *tmp = malloc(sizeof(char) * length);
    int tmp_code = 1;
    for(int i = 0; i < length; i++){
        if(decimal_codes.binary_code[i] == '1'){
            tmp_code = tmp_code * 2 + 1;
        }
        else{
            tmp_code = tmp_code * 2;
        }
    }
//    printf("%d\n", tmp_code);
    return tmp_code;
}

int to_decimal(int n)
{
    int decimalNumber = 0, i = 0, reszta;
    while (n!=0)
    {
        reszta = n%10;
        n /= 10;
        decimalNumber += reszta*pow(2,i);
        ++i;
    }
    return decimalNumber;
}

void decoder(Output *codes, char *data, int n, int version, char *rest2){
    FILE *out = fopen("../Compressor-Decompressor/testowy_output.txt","wb");
    decode_t *decimal_codes= malloc(sizeof(decode_t) * n);

    for(int i = 0; i < n; i++){
        decimal_codes[i].bits = codes[i].bits;
        decimal_codes[i].binary_code = codes[i].code;
        decimal_codes[i].decimal_code = decimal_code_maker(decimal_codes[i]);
    }
    int maxi = decimal_codes[0].decimal_code;
    for(int i = 1; i < n; i++){
        if(maxi < decimal_codes[i].decimal_code){
            maxi = decimal_codes[i].decimal_code;
        }
    }
    maxi++;
//    printf("nasze maxi to: %d\n", maxi);

    decode_t2 *final_codes = malloc(sizeof(decode_t2) * maxi);
    for(int i = 0; i < maxi; i++){
        final_codes[i].bits = -1;
        final_codes[i].decimal_code = -1;
        final_codes[i].exist = false;
    }
    for(int i = 0; i < n; i++){
        if (decimal_codes[i].decimal_code >= 0 && decimal_codes[i].decimal_code < maxi) {
            final_codes[decimal_codes[i].decimal_code].bits = decimal_codes[i].bits;
            final_codes[decimal_codes[i].decimal_code].exist = true;
        }
        else {
            printf("proba wyjscia poza tablice na kodzie decymalnym %d, a binarnym %s", decimal_codes[i].decimal_code, decimal_codes[i].binary_code);
            return;
        }
    }


    int length = strlen(data);
    int tmp = 1;
    for(int i = 0; i < length; i++){
        if(data[i] == '1'){
            tmp = tmp * 2 + 1;
        }
        else{
            tmp = tmp * 2;
        }
        if(final_codes[tmp].exist == true){
            writeToFile(&final_codes[tmp].bits,out,version);
//            printf("%c",final_codes[tmp].bits);
            tmp = 1;
        }
    }
    if(*rest2)
        fwrite(rest2,sizeof(char),1,out);

}

