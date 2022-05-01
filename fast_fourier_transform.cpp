#include <stdint.h>
#include <math.h>

#include "fast_fourier_transform.h"

#define M_PI    3.14159265358979323846F

void shuffle_indicies(uint16_t indicies[], unsigned int exponent)
{
    for (unsigned int idx = 0; idx < (1U << exponent); idx ++)
    {
        unsigned int reverse_idx = 0;
        for (unsigned int i = 0; i < exponent; i ++)
        {
            reverse_idx <<= 1;
            reverse_idx |= ((idx >> i) & 1U);
        }
        indicies[idx] = reverse_idx;
    }
}

// indicies and exponent must be passed as arguments despite being private members of this class,
// since they are modified on each recursive function call
inline float FFT::recursive_DFT(uint16_t indicies[], unsigned int exponent, unsigned int f)
{
    unsigned int shifted_f = (f << (MAX_EXPONENT - exponent));
    //float invertor = ((shifted_f & 0b11000000000000) % 0b11) ? -1.0F : 1.0F;
    float invert = ((shifted_f & 0b11000000000000) % 0b11);
    unsigned int index_mask = ((1U << (MAX_EXPONENT - 1)) - 1);
    unsigned int coeff_idx = (shifted_f & 0b1000000000000) ? ~(shifted_f - 1) : shifted_f;
    coeff_idx &= index_mask;

    if (exponent > 1)
    {
        exponent --;
        //float even = recursive_DFT(indicies, exponent, f);
        //float odd = recursive_DFT(indicies + (1 << exponent), exponent, f);
        //return even + (odd * invertor * _coefficients[coeff_idx]);
        //return recursive_DFT(indicies, exponent, f) + (recursive_DFT(indicies + (1 << exponent), exponent, f) * invertor * _coefficients[coeff_idx]);
        if (invert)
        {
            return recursive_DFT(indicies, exponent, f) - (recursive_DFT(indicies + (1 << exponent), exponent, f) * _coefficients[coeff_idx]);
        }
        else
        {
            return recursive_DFT(indicies, exponent, f) + (recursive_DFT(indicies + (1 << exponent), exponent, f) * _coefficients[coeff_idx]);
        }
    }

    if (invert)
    {
        return _samples[indicies[0]] - (_samples[indicies[1]] * _coefficients[coeff_idx]);
    }
    else
    {
        return _samples[indicies[0]] + (_samples[indicies[1]] * _coefficients[coeff_idx]);
    }
    //return _samples[indicies[0]] + (_samples[indicies[1]] * invertor * _coefficients[coeff_idx]);
}

FFT::FFT()
{
    // precompute coefficients for the maximum FFT length
    // The FFT length is represented by an exponent, since it must alway be a power of 2
    // To save memory and exploint the symetries of the coefficients calculation, only a quater of the coefficients are calculated.
    // Precomputing the coefficients in this way discards any of the complex (phase) information in the FFT. Might change this later.
    // The iterator f represents the frequency component.
    unsigned int max_coefficients = 1U << (MAX_EXPONENT - 2);
    unsigned int max_FFT_length = 1U << MAX_EXPONENT;
    unsigned int f;
    for (f = 0; f < max_coefficients; f ++)
    {
        _coefficients[f] = cos((2.0F * M_PI * static_cast<float>(f)) / static_cast<float>(max_FFT_length));
    }
    _coefficients[f] = 0.0F;
}

void FFT::setup(float samples[], unsigned int num_samples)
{
    _samples = samples;
    _num_samples = num_samples; 
    while (num_samples --)
    {
        num_samples >>= 1;
        _exponent ++;
    }
    _FFT_length = (1U << _exponent);
    shuffle_indicies(_indicies, _exponent);
}
    
float FFT::compute_component(unsigned int f)
{
    return recursive_DFT(_indicies, _exponent, f);
}

