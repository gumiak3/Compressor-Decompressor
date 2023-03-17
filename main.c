#include <stdio.h>
#include <stdlib.h>
#include "dataReader.h"
#include "splitData.h"
int main(int argc, char**argv) {
    int size = 0;
    char *data = readData(argv[1],&size);
    char rest;
    short *splittedData = splitData(data,&size,16,&rest);
    printf("size %d\n", size);
    for(int i=0;i<size;i++){
        printf("%c, %c", splittedData[i] >> 8, splittedData[i]);
    }
    printf(",%c", rest);
    return 0;

}
