#ifndef COMPRESSOR_PROBABILITY_H
#define COMPRESSOR_PROBABILITY_H
typedef struct{
    char *word;
    int amount;
}probability_t;

probability_t  * readBinaryFile(char *fileName, int flag);
void sortArray(probability_t *array);

#endif //COMPRESSOR_PROBABILITY_H
