#ifndef COMPRESSOR_PROBABILITY_H
#define COMPRESSOR_PROBABILITY_H
typedef struct{
    short bits;
    int amount;
}probability_t;

probability_t  * readBinaryFile(char *, int );
void sortArray(probability_t *);
int getSize(probability_t *);
#endif //COMPRESSOR_PROBABILITY_H
