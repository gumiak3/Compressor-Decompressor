#include <stdio.h>
#include <stdlib.h>
#include "dataReader.h"
#include "splitData.h"
int main(int argc, char**argv) {
    int size = 0;
    char *data = readData(argv[1],&size);
    short rest;
    short *splittedData = splitData(data,&size,16,&rest);
    for(int i=0;i<size;i++){
        printf("%c ", splittedData[i]>>8);
        printf("%c\n", splittedData[i]);
    }
    printf("rest: %d",rest);
    return 0;

}
