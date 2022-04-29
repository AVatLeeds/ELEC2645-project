#include <cstdio>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

float recursive_FFT(float samples[], float holder[], int N, int f)
{
    for (int i = 0; i < (N / 2); i ++)
    {
        holder[i] = samples[i << 1];
    }
    for (int i = 0; i < (N / 2); i ++)
    {
        holder[i + (N / 2)] = samples[(i << 1) + 1];
    }

    if (N > 2)
    {
        return recursive_FFT(holder, samples, (N >> 1), f) + (recursive_FFT(holder + (N / 2), samples + (N / 2), (N >> 1), f) / cos((2 * 3.14159 * f) / N));
    }
    else
    {
        return samples[0] + (samples[1] / cos((2 * 3.14159 * f) / N));
    }
}

#define NUM_SAMPLES 32

void pointer_shuffle(float samples[], float * pointers[], int exponent)
{
    for(int idx = 0; idx < (1 << exponent); idx ++)
    {
        int reverse_idx = 0;
        for (int i = 0; i < exponent; i ++)
        {
            reverse_idx <<= 1;
            reverse_idx |= ((idx >> i) & 0b1);   
        }
        pointers[idx] = &samples[reverse_idx];
    }
}
    
float recursive_DFT(float * pointers[], int length, int f)
{
    if (length > 2)
    {
        return (recursive_DFT(pointers, length >> 1, f) + (cos((2 * 3.14159 * f) / length) * recursive_DFT(pointers + (length >> 1), length >> 1, f)));
    }
    else
    {
        return (*pointers[0] + (cos((2 * 3.14159 * f) / length) * *pointers[1]));
    }
}

int main()
{
    /*** FFT algorithm tests ***/

    float signal1[NUM_SAMPLES] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    float signal2[NUM_SAMPLES] = {1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1};
    float signal3[NUM_SAMPLES] = {1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1};
    float signal4[NUM_SAMPLES] = {1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0};
    float signal5[NUM_SAMPLES] = {1.2, 1.2, 1.2, 1.2, -0.8, -0.8, -0.8, -0.8, 1.2, 1.2, 1.2, 1.2, -0.8, -0.8, -0.8, -0.8, 1.2, 1.2, 1.2, 1.2, -0.8, -0.8, -0.8, -0.8, 0, 0, 0, 0, 0, 0, 0, 0};
    float signal6[NUM_SAMPLES] = {1, 1, 1, 1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    float * pointers[NUM_SAMPLES];

    float list[NUM_SAMPLES] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
    pointer_shuffle(signal6, pointers, 5);

    for (int i = 0; i < NUM_SAMPLES; i ++)
    {
        printf("%f\n", *pointers[i]);
    }
    printf("\n");
    
    for (int f = 0; f < NUM_SAMPLES; f ++)
    {
        float component = recursive_DFT(pointers, NUM_SAMPLES, f);
        printf("%f\n", component);
    }
    


    /*
    float samples[32] = {1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1};
    float coefficients[32];
    for (int i = 0; i < 32; i ++)
    {
        coefficients[i] = cos((2 * 3.14159 * i) / 32);
        printf("%f\n", coefficients[i]);
    }
    printf("\n");

    float holder[32];
    float list[32] = {1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1};

    for (int f = 0; f < 16; f ++)
    {
        list = {1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1};
        printf("%f\n", recursive_FFT(list, holder, 16, f));
    }
    */

    return 0;
}

