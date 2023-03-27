#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "decompressor.h"
//typedef struct input{
//    short bits;
//    char *code;
//}input;

typedef struct decode_t{
    short bits;
    int decimal_code;
    char *binary_code;
} decode_t;

typedef struct decode_t2{
    short bits;
    int decimal_code;
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

void decoder(Output *codes, char *data, int n, int version){
    FILE *out = fopen("../Compressor-Decompressor/nowy.txt","wb");
    decode_t *decimal_codes= malloc(sizeof(decode_t) * n);

    for(int i = 0; i < n; i++){
        decimal_codes[i].bits = codes[i].bits;
        decimal_codes[i].binary_code = codes[i].code;
        decimal_codes[i].decimal_code = decimal_code_maker(decimal_codes[i]);
    }

    decode_t2 *final_codes = malloc(sizeof(decode_t2) * pow(2, version));
    for(int i = 0; i < pow(2, 8); i++){
        final_codes[i].bits = -1;
        final_codes[i].decimal_code = -1;
    }
    for(int i = 0; i < n; i++){
        final_codes[decimal_codes[i].decimal_code].bits = decimal_codes[i].bits;
        final_codes[decimal_codes[i].decimal_code].decimal_code = decimal_codes[i].decimal_code;
    }
//    for(int i = 0; i < pow(2, 4); i++){
//        if(final_codes[i].bits != -1){
//            printf("bits: %d decimal_code %d \n", final_codes[i].bits, final_codes[i].decimal_code);
//        }
//    }

    int length = strlen(data);
    int tmp = 1;
    for(int i = 0; i < length; i++){
        if(data[i] == '1'){
            tmp = tmp * 2 + 1;
        }
        else{
            tmp = tmp * 2;
        }
//        printf("%d ", tmp);
        if(final_codes[tmp].bits != -1){
            printf("%d", final_codes[tmp].bits);
            // wpisywanie do pliku
            tmp = 1;
        }

    }
//    printf("\n");
//    printf("%d\n",n);
//    for(int i = 0; i < n; i++){
//        printf("bits: %d binary_code: %s decimal_code %d \n", decimal_codes[i].bits, decimal_codes[i].binary_code, decimal_codes[i].decimal_code);
//    }
}


//int main(int argc, char **argv) {
//    int n = 3;
//    struct input *codes = malloc(sizeof(input) * n);
//    codes[0].bits = 97;
//    codes[0].code = "01";
//    codes[1].bits = 98;
//    codes[1].code = "00";
//    codes[2].bits = 99;
//    codes[2].code = "1";
//    int version = 8;
////    for(int i = 0; i < n; i++){
////        printf("index: %d, bits: %d, code: %s\n", i, codes[i].bits, codes[i].code);
////    }
//    char *data;
//    data = malloc(86 * sizeof(char)) ;
//    if (data == NULL) {
//        printf("Error: memory allocation failed");
//        exit(1);
//    }
//    strcpy(data, "01001");
//    decoder(codes, data, n, version);
//    //    printf("%s\n", data);
//}