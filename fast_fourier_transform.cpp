#include <stdint.h>
#include <math.h>

#include "fast_fourier_transform.h"

#define M_PI    3.14159265358979323846F

// reverses the bits of each index and stores the result in the indices array
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

float * FFT::compute_FFT()
{
    // populate the results array with the sums and differences of each pair of shuffled samples
    for (unsigned int f = 0; f < _FFT_in_length; f += 2)
    {
        unsigned int even_idx = _indicies[f];
        unsigned int odd_idx = _indicies[f + 1];
        _results[f] = _samples[even_idx] + _samples[odd_idx];
        _results[f + 1] = _samples[even_idx] - _samples[odd_idx];
    }
    // a second pass over the array takes advantage of a special case that 4 evenly spaced points
    // on the unit circle yield coefficients with real part 1.0, 0,0, -1.0 and 0.0
    for (unsigned int f = 0; f < _FFT_in_length; f += 4)
    {
        float sum = _results[f] + _results[f + 2];
        float diff = _results[f] - _results[f + 2];
        _results[f] = sum;
        _results[f + 2] = diff;
    }
    // the two for loops above take care of the conditions that would be width = 0 and width = 1
    unsigned int width = 2;
    // each value of width here is equivalent to a layer on the butterfly diagrem.
    while (width < _FFT_in_width)
    {
        // this value is used to shift (raise to a power of 2) the frequency component f so that
        // it correctly sub-samples the coefficients array
        unsigned int coeff_shift = MAX_EXPONENT - (width + 1);
        unsigned int offset = (1U << width);
        for (unsigned int f = 0; f < _FFT_in_length; f ++)
        {
            if (f & offset)
            {
                f += offset;
            }
            float temp = (_coefficients[(f << coeff_shift) & _coeff_idx_mask] * _results[f + offset]);
            float diff = _results[f] - temp;
            _results[f] = _results[f] + temp;
            _results[f + offset] = diff;     
        }
        width ++;
    }

    return _results;
}

FFT::FFT()
{
    // precompute coefficients for the maximum FFT length
    // The FFT length is represented by an exponent, since it must alway be a power of 2
    // To save memory and exploint the symetries of the coefficients calculation, only half of the coefficients are calculated.
    // Precomputing the coefficients in this way discards any of the complex (phase) information in the FFT. Might change this later.
    // The iterator f represents the frequency component.
    unsigned int max_FFT_length = 1U << MAX_EXPONENT;
    for (unsigned int f = 0; f < _num_coefficients; f ++)
    {
        _coefficients[f] = cos((2.0F * M_PI * static_cast<float>(f)) / static_cast<float>(max_FFT_length));
    }
}

void FFT::setup(volatile float samples[], unsigned int num_samples)
{
    _samples = samples;
    _num_samples = num_samples; 
    while (num_samples --)
    {
        num_samples >>= 1;
        _FFT_in_width ++;
    }
    _FFT_in_length = (1U << _FFT_in_width);
    shuffle_indicies(_indicies, _FFT_in_width);
}
    
float FFT::compute_component(unsigned int f)
{
    return recursive_DFT(_indicies, _FFT_in_width, f);
}

