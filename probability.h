#ifndef COMPRESSOR_PROBABILITY_H
#define COMPRESSOR_PROBABILITY_H
typedef struct{
    char *word;
    int amount;
}probability_t;

probability_t  * readBinaryFile(char *fileName, int flag);


#endif //COMPRESSOR_PROBABILITY_H
