
#ifndef COMPRESSOR_FREQUENCY_H
#define COMPRESSOR_FREQUENCY_H
typedef struct frequency_t{
    short bits;
    int frequency;
    int isTrue;
}frequency_t;

frequency_t * getFrequency(short *data, int *size);

#endif //COMPRESSOR_FREQUENCY_H
