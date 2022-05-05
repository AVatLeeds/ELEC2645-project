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

//inline void FFT::compute_FFT(uint16_t indicies[], unsigned int width)
//{
//    if (width > 2)
//    {
//        //descend the recursion
//        width --;
//        compute_FFT(indicies, width);
//        compute_FFT(indicies + (1 << width), width);
//
//        //ascend back up the recursion collecting terms as we go
//    }
//    else
//    {
//        // we have reached the bottom of the recursion
//        float even = _samples[_indicies[0]] + _samples[_indicies[1]];
//        float odd = _samples[_indicies[0]] - _samples[_indicies[1]];
//
//        //populate all the even frequency terms
//        for (unsigned int f = 0; f < _FFT_out_length; f += 2)
//        {
//            _samples[f] = even;
//        }
//
//        //populate all the odd frequency terms
//        for (unsigned int f = 1; f < _FFT_out_length; f += 2)
//        {
//            _samples[f] = odd;
//        }
//    }
//}

//void FFT::compute_FFT(unsigned int width)
//{
//    if (width > 2)
//    {
//        //descend the recursion
//        compute_FFT(--width);
//
//        unsigned int offset = 1U << (width - 2);
//        unsigned int index_mask = ((1U << (MAX_EXPONENT - 1)) - 1);
//
//        for (unsigned int f = 0; f < _FFT_in_length; f ++)
//        {
//            //unsigned int shifted_f = (f << (MAX_EXPONENT - width));
//            //float invert = ((shifted_f & 0b11000000000000) % 0b11);
//            //unsigned int coeff_idx = (shifted_f & 0b1000000000000) ? ~(shifted_f - 1) : shifted_f;
//            //coeff_idx &= index_mask;
//            if (f & offset)
//            {
//                _samples[f] = _samples[f - offset] - (cos((-2.0 * M_PI * f) / _FFT_in_length) * _samples[f]); 
//            }
//            else
//            {
//                _samples[f] = _samples[f] + (cos((-2.0 * M_PI * f) / _FFT_in_length) * _samples[f + offset]);
//            }
//        }
//
//        //ascend back up the recursion collecting terms as we go
//    }
//    else if (width > 1)
//    {
//        compute_FFT(--width);
//        // this layer takes advantage of a special case that
//        // 4 evenly spaced points on the unit circle yield coefficients
//        // with real part 1.0, 0,0, -1.0 and 0.0
//        for (unsigned int f = 0; f < _FFT_in_length; f += 4)
//        {
//            float sum = _samples[f] + _samples[f + 2];
//            _samples[f] = sum;
//            _samples[f + 2] = sum;
//        }
//    }
//    else
//    {
//        // we have reached the bottom of the recursion
//        // overwrite the entries in the samples sequence with their sums and their differences
//        for (unsigned int f = 0; f < _FFT_in_length; f += 2)
//        {
//            unsigned int even_idx = _indicies[f];
//            unsigned int odd_idx = _indicies[f + 1];
//            float sum = _samples[even_idx] + _samples[odd_idx];
//            float diff = _samples[even_idx] - _samples[odd_idx];
//            _samples[f] = sum;
//            _samples[f + 1] = diff;
//        }
//    }
//}

void FFT::compute_FFT(unsigned int fft_width)
{
    // overwrite the entries in the samples sequence with their sums and their differences
    for (unsigned int f = 0; f < _FFT_in_length; f += 2)
    {
        unsigned int even_idx = _indicies[f];
        unsigned int odd_idx = _indicies[f + 1];
        _results[f] = _samples[even_idx] + _samples[odd_idx];
        _results[f + 1] = _samples[even_idx] - _samples[odd_idx];
    }
    // a second pass over the array takes advantage of a special case that
    // 4 evenly spaced points on the unit circle yield coefficients
    // with real part 1.0, 0,0, -1.0 and 0.0
    for (unsigned int f = 0; f < _FFT_in_length; f += 4)
    {
        float sum = _results[f] + _results[f + 2];
        float diff = _results[f] - _results[f + 2];
        _results[f] = sum;
        _results[f + 2] = diff;
    }
    
    unsigned int width = 2;
    while (width < fft_width)
    {
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
}



FFT::FFT()
{
    // precompute coefficients for the maximum FFT length
    // The FFT length is represented by an exponent, since it must alway be a power of 2
    // To save memory and exploint the symetries of the coefficients calculation, only a quater of the coefficients are calculated.
    // Precomputing the coefficients in this way discards any of the complex (phase) information in the FFT. Might change this later.
    // The iterator f represents the frequency component.
    unsigned int max_FFT_length = 1U << MAX_EXPONENT;
    unsigned int f;
    for (f = 0; f < _num_coefficients; f ++)
    {
        _coefficients[f] = cos((2.0F * M_PI * static_cast<float>(f)) / static_cast<float>(max_FFT_length));
    }
}

void FFT::setup(float samples[], unsigned int num_samples)
{
    _samples = samples;
    _num_samples = num_samples; 
    while (num_samples --)
    {
        num_samples >>= 1;
        _FFT_in_width ++;
    }
    _FFT_in_length = (1U << _FFT_in_width);
    _FFT_out_width = _FFT_in_width - 1;
    _FFT_out_length = (1U << _FFT_out_width);
    shuffle_indicies(_indicies, _FFT_in_width);
}
    
float FFT::compute_component(unsigned int f)
{
    return recursive_DFT(_indicies, _FFT_in_width, f);
}

