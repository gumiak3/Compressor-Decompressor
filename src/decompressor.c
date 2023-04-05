#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdint-gcc.h>
#include "decompressor.h"

typedef struct Node{
    short bits;
    struct Node *left;
    struct Node *right;
    bool is_leaf;
} Node;

void write8Bits(short *bits, FILE *out){
    fwrite(bits,1,1,out);
}

void write12Bits(short *bits, FILE *out, unsigned char *buffor, int *bufforIndex){
    // still working on it
    if(*bufforIndex==0){
        // przesuwamy w prawo, wpisujemy do pliku, a nastepnie reszta ktora zostala zapisujemy do buffora i zmieniamy index na 4
        short toWrite = (*bits >> 4) & 0x00FF;
        *buffor = (*bits << 4);
        write8Bits(&toWrite,out);
        *bufforIndex = 4;
    }else{
        // bierzemy z buffora 4, dokladamy do buffora nastepna 4, wpisujemy buffor do pliku, a nastepnie 8 bitow ktore zostalo
        short toWrite = *buffor | ((*bits >> 8) & 0x00FF);
        write8Bits(&toWrite,out);
        write8Bits(bits,out);
        *bufforIndex = 0;
    }

}

void write16Bits(short *bits, FILE *out){
    short swapped = ((*bits & 0xFF) << 8) | ((*bits >> 8) & 0xFF); // bitwise operations to swap the bits
    fwrite(&swapped,sizeof(short),1,out);
}

void writeToFile(short *bits, FILE *out, int compressionRatio, unsigned char *buffor, int *bufforIndex){
    switch(compressionRatio){
        case 8:
            write8Bits(bits,out);
            break;
        case 12:
            write12Bits(bits,out,buffor, bufforIndex);
            break;
        case 16:
            write16Bits(bits,out);
            break;
    }
}


void freeTree(Node *root){
    if(root->left != NULL){
        freeTree(root->left);
    }
    if(root->right != NULL){
        freeTree(root->right);
    }
    free(root);
}

void decoder(Output *codes, char *data, int n, int version, char *rest2, int restControl,FILE *out){


    Node *root = malloc(sizeof(Node));
    root->left = NULL;
    root->right = NULL;
    root->is_leaf = false;

    for(int i = 0; i < n; i++){
        Node *tmp = root;
        int code_length = strlen(codes[i].code);
        for(int j = 0; j < code_length; j++){
            if(codes[i].code[j] == '0'){
                if(tmp->left == NULL){
                    tmp->left = malloc(sizeof(Node));
                    tmp->left->left = NULL;
                    tmp->left->right = NULL;
                    tmp->left->is_leaf = false;
                }
                tmp = tmp->left;
            }
            else{
                if(tmp->right == NULL){
                    tmp->right = malloc(sizeof(Node));
                    tmp->right->left = NULL;
                    tmp->right->right = NULL;
                    tmp->right->is_leaf = false;
                }
                tmp = tmp->right;
            }
        }
        tmp->bits = codes[i].bits;
        tmp->is_leaf = true;
    }
    unsigned char buffor;
    int bufforIndex = 0;
    int length = strlen(data);
    Node *current = root;

    for(int i = 0; i < length; i++){
        if(data[i] == '0'){
            current = current->left;
        }
        else{
            current = current->right;
        }
        if(current->is_leaf == true){
            writeToFile(&current->bits,out,version,&buffor,&bufforIndex);
            current = root;
        }
    }

    switch(restControl){
        case 4:
            buffor |= ((*rest2>>4) & 0x000F);
            write8Bits((short*)&buffor,out);
            break;
        case 8:
            write8Bits((short*)rest2,out);
            break;
    }

    freeTree(root);

}

