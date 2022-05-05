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

    #define NUM_SAMPLES 1024

    float signal1[NUM_SAMPLES] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
                                -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 ,-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1};
    float signal2[NUM_SAMPLES] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1};

    GPIO_pin display_trigger(PORTA, 1);
    display_trigger.mode(OUTPUT);

    FFT fft; // constructor precomputes coefficients
    status = BLUE;
    fft.setup(signal1, NUM_SAMPLES);

    status = CYAN;

    float * results;

    while (true)
    {
        display_trigger.toggle(); // set trigger before computing FFT
        results = fft.compute_FFT();
        display_trigger.toggle(); // clear trigger after computing FFT
        //display the samples
        for (unsigned int f = 0; f < NUM_SAMPLES; f ++)
        {
            DAC.registers->CH1_data_reg_12bit_right_allign = (uint16_t)((results[f] * 100) + 1);
        }
    }
}