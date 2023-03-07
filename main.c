#include <stdio.h>
#include <stdlib.h>
#include "probability.h"
int main(int argc, char**argv) {
    probability_t *  probability = readBinaryFile(argv[1], atoi(argv[2])/8);
    for(int i=0;i<12;i++){
        printf("%s : %d\n",probability[i].word, probability[i].amount);
    }

    return 0;
}
