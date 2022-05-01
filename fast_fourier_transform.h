#ifndef FAST_FOURIER_TRANSFORM_H
#define FAST_FOURIER_TRANSFORM_H

#include <stdint.h>

#define MAX_EXPONENT 14

class FFT
{
    public:

    FFT();

    void setup(float samples[], unsigned int num_samples);
    float compute_component(unsigned int f);

    //private:

    float recursive_DFT(uint16_t indicies[], unsigned int exponent, unsigned int f) __attribute__((optimize("-O3")));

    unsigned int _num_samples = 0;
    unsigned int _FFT_length = 0;
    unsigned int _exponent = 0;
    unsigned int _FFT_length_bitmask = ((1U << MAX_EXPONENT) - 1);
    unsigned int _num_coefficients = (1U << (MAX_EXPONENT - 2)) + 1U;
    // should probably be pre-computed and then externally linked but this will do for now.
    float _coefficients[(1U << (MAX_EXPONENT - 2)) + 1U];
    uint16_t _indicies[1U << MAX_EXPONENT];
    float * _samples;

};

#endif