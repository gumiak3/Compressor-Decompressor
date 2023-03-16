#include <stdio.h>
#include <stdlib.h>
#include "probability.h"
int main(int argc, char**argv) {
    probability_t *  probability = readBinaryFile(argv[1], atoi(argv[2]));
    for(int i=0;i<getSize(probability);i++){
        if(probability[i].bits){
            printf("%d \n",probability[i].bits >> 4);
            printf("%d \n",probability[i].bits );
        }

    }
    printf("size: %d \n", getSize(probability)); // doesn't work on 12 bits yet!
    return 0;
}
