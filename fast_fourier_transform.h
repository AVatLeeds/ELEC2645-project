#ifndef FAST_FOURIER_TRANSFORM_H
#define FAST_FOURIER_TRANSFORM_H

#include <stdint.h>

#define MAX_EXPONENT 13

class FFT
{
    public:

    FFT();

    void setup(float samples[], unsigned int num_samples);
    float compute_component(unsigned int f);
    void compute_FFT(unsigned int width);

    //private:

    float recursive_DFT(uint16_t indicies[], unsigned int exponent, unsigned int f);

    unsigned int _num_samples = 0;

    unsigned int _FFT_in_length = 0;
    unsigned int _FFT_in_width = 0;
    unsigned int _FFT_out_length = 0;
    unsigned int _FFT_out_width = 0;

    unsigned int _FFT_length_bitmask = ((1U << MAX_EXPONENT) - 1);
    unsigned int _num_coefficients =1U << (MAX_EXPONENT - 1);
    unsigned int _coeff_idx_mask = ((1U << (MAX_EXPONENT - 1)) - 1);

    // should probably be externally linked but this will do for now.
    float _coefficients[1U << (MAX_EXPONENT - 1)];
    uint16_t _indicies[1U << MAX_EXPONENT];
    float * _samples;
    float _results[1U << MAX_EXPONENT];

};

#endif