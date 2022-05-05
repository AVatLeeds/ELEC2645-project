#include <stdint.h>
#include <stddef.h>
#include <cmath>

#include "status_LED.h"
#include "system.h"
#include "systick.h"
#include "GPIO_driver.h"
#include "USART_driver.h"
#include "SPI_driver.h"
#include "DAC_driver.h"
#include "fast_fourier_transform.h"

// temporary for experiments only
void nop() __attribute__((optimize(0)));
void nop()
{
    __asm__("nop");
}

status_LED status(PORTA, 13, 15, 14);

USART_driver uart(USART_2, 115200);

SPI_driver SPI(SPI1);

DAC_driver DAC;

void setup_uart_pins() // this needs to go in the USART class constructor when I have time
{
    GPIO_ENABLE(PORTA);
    // USART2 transmit and receive are on pins 2 and 3 of PORTA
    GPIO_SET_MODE(PORTA, 2, ALTERNATE_FUNCTION);
    GPIO_SET_MODE(PORTA, 3, ALTERNATE_FUNCTION);
    GPIO_SET_TYPE(PORTA, 2, PUSH_PULL);
    GPIO_SET_TYPE(PORTA, 3, PUSH_PULL);
    GPIO_SET_SPEED(PORTA, 2, HIGH_SPEED);
    GPIO_SET_SPEED(PORTA, 3, HIGH_SPEED);
    // alternate function 7 is USART
    GPIO_SET_ALTERNATE_FUNCTION(PORTA, 2, AF7);
    GPIO_SET_ALTERNATE_FUNCTION(PORTA, 3, AF7);
}

void setup_SPI()
{
    /* SPI pins moved to port B to avoid conflict with DAC outputs
        - SCK - PB3
        - MISO - PB4
        - MOSI - PB5
    */

    GPIO_ENABLE(PORTA);

    //GPIO_SET_MODE(PORTA, 4, ALTERNATE_FUNCTION); NSS functionality not needed for ADC
    GPIO_SET_MODE(PORTB, 3, ALTERNATE_FUNCTION);
    GPIO_SET_MODE(PORTB, 4, ALTERNATE_FUNCTION);
    GPIO_SET_MODE(PORTB, 5, ALTERNATE_FUNCTION);

    //GPIO_SET_ALTERNATE_FUNCTION(PORTA, 4, 5); NSS functionality not needed for ADC
    GPIO_SET_ALTERNATE_FUNCTION(PORTB, 3, 5);
    GPIO_SET_ALTERNATE_FUNCTION(PORTB, 4, 5);
    GPIO_SET_ALTERNATE_FUNCTION(PORTB, 5, 5);

    SPI.baud_rate_divisor(0); // this is actually a divisor of 2. Update the method so that the numbers match
    SPI.clock_polarity(1);
    SPI.clock_phase(1);
    SPI.frame_MSB_first();
    SPI.CRC_enabled(false);
    SPI.master_mode();
    SPI.data_size(14);
    SPI.SS_output_enable(true);
    SPI.FIFO_threshold_16bit();

    SPI.enable();
}

/*
void indexing_algorithm(unsigned int exponent, unsigned int f)
{
    unsigned int shifted_f = (f << (MAX_EXPONENT - exponent));
    float invertor = ((shifted_f & 0b11000000000000) % 0b11) ? -1.0 : 1.0;
    unsigned int coeff_idx = shifted_f & ((1U << (MAX_EXPONENT - 1)) - 1);
    if (coeff_idx > (1U << (MAX_EXPONENT - 2)))
    {
        coeff_idx = ~(coeff_idx - 1) & ((1U << (MAX_EXPONENT - 1)) - 1);
    }
    //uart.print_register("shifted idx", coeff_idx);
    uart.print_register("invertor", static_cast<int8_t>(invertor));
    delay_ms(100);
}
*/

int main(void)
{
    systick_init();
    setup_uart_pins();
    setup_SPI(); 

    status = PURPLE;
    delay_ms(1000);

    GPIO_pin ADC_conv(PORTA, 4);
    ADC_conv.mode(OUTPUT);
    ADC_conv.type(PUSH_PULL);

    status = YELLOW;

    DAC.mode_normal(CH1, false, false);
    DAC.enable(CH1);

    /*
    float precomputed[4096];
    
    for (uint16_t i = 0; i < (1 << 12); i ++)
    {
        precomputed[i] = sin((static_cast<float>(i) * 2 * 3.1415926) / 1024) / ((static_cast<float>(i) * 2 * 3.1415926) / 1024);
    }

    uint16_t precomputed_int[4096];
    
    for (uint16_t i = 0; i < (1 << 12); i ++)
    {
        //precomputed_int[i] = static_cast<uint16_t>((precomputed[i] + 1) * 2047);
        precomputed_int[i] = static_cast<uint16_t>(abs(precomputed[i]) * 2047);
    }
    */

    #define NUM_SAMPLES 1024

    float signal1[NUM_SAMPLES] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
                                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 ,-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    float signal2[NUM_SAMPLES] = {1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1};
    //float signal3[NUM_SAMPLES] = {1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1};
    //float signal4[NUM_SAMPLES] = {1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0};
    //float signal5[NUM_SAMPLES] = {1.2, 1.2, 1.2, 1.2, -0.8, -0.8, -0.8, -0.8, 1.2, 1.2, 1.2, 1.2, -0.8, -0.8, -0.8, -0.8, 1.2, 1.2, 1.2, 1.2, -0.8, -0.8, -0.8, -0.8, 0, 0, 0, 0, 0, 0, 0, 0};
    //float signal6[NUM_SAMPLES] = {1, 1, 1, 1, -1, -1, -1, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    //float signal7[NUM_SAMPLES] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //                            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    //                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //                            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    //                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //                            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    //                            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    //                            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};


    GPIO_pin display_trigger(PORTA, 1);
    display_trigger.mode(OUTPUT);

    FFT fft;
    status = BLUE;
    fft.setup(signal2, NUM_SAMPLES);

    status = CYAN;

    uart.newline();
    //for (unsigned int f = 0; f < (NUM_SAMPLES >> 1); f ++)
    //{
    //    indexing_algorithm(fft._exponent, f);
    //}

    while (true)
    {
        //for (unsigned int f = 0; f < (NUM_SAMPLES >> 1); f ++)
        //{
        //    FFT_out[f] = static_cast<uint16_t>(fft.compute_component(f) * 300);
        //}
        display_trigger.toggle();
        /*
        for (uint16_t i = 0; i < (1 << 12); i ++)
        {
            //DAC.write(ALIGN_RIGHT, CH1, i);
            //DAC.registers->CH1_data_reg_12bit_right_allign = i;
            DAC.registers->CH1_data_reg_12bit_right_allign = precomputed_int[i];
        }
        */

        //for (unsigned int i = 0; i < fft._num_coefficients; i ++)
        //{
        //    DAC.registers->CH1_data_reg_12bit_right_allign = static_cast<uint16_t >(fft._coefficients[i] * 4095);
        //}

        //for (unsigned int i = 0; i < (fft._FFT_length << 4); i ++)
        //{
        //    DAC.registers->CH1_data_reg_12bit_right_allign = (fft._indicies[(i >> 4)] * 100);
        //}
        //DAC.registers->CH1_data_reg_12bit_right_allign = 0;
        fft.compute_FFT(10);
        display_trigger.toggle();
        for (unsigned int f = 0; f < NUM_SAMPLES; f ++)
        {
            DAC.registers->CH1_data_reg_12bit_right_allign = (uint16_t)((fft._results[f] + 1.0) * 100);
            nop();
            nop();
            nop();
            nop();
            nop();
        }
        //for (unsigned int f = 0; f < NUM_SAMPLES; f ++)
        //{
        //    fft._samples[f] = 0.0;//(static_cast<float>((f >> 2) & 1) * 2.0) - 1.0;
        //}
        //fft._samples[0] = 1;
        //fft._samples[1] = 1;
        //fft._samples[2] = 1;
        //fft._samples[3] = 1;
        //fft._samples[4] = -1;
        //fft._samples[5] = -1;
        //fft._samples[6] = -1;
        //fft._samples[7] = -1;
        


        //status = RED;

        //ADC_conv.set();
        //ADC_conv.clear();

        //SPI.registers->data_reg = 0x0000;
        //uint16_t data = SPI.registers->data_reg;
    }
}