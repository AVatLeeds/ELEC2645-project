#include <stdint.h>
#include <stddef.h>
#include <math.h>

#include "status_LED.h"
#include "system.h"
#include "systick.h"
#include "GPIO_driver.h"
#include "USART_driver.h"
#include "SPI_driver.h"
#include "timers.h"
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

Basic_timer sample_timer(TIMER6);

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

    GPIO_ENABLE(PORTB);

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

void setup_sample_timer()
{
    sample_timer.interrupt_on_update(true);
    sample_timer.remap_update_interrupt_flag_to_count(true);
    sample_timer.set_prescaler(4); // prescale down to 1 MHz
    sample_timer.set_threshold(25); // timer threshold divides by a further 25, to get 40 KHz
    sample_timer.clear_update_interrupt_flag();
    sample_timer.start();
}

#define NUM_SAMPLES 1024
// these need to be visible to the ISR
volatile bool display_flag = false; // can be changed by the ISR
volatile unsigned int sample_idx = 0; // can be changed by the ISR
volatile float samples[NUM_SAMPLES];
GPIO_pin ADC_conv(PORTA, 0);

int main(void)
{
    float * results;

    systick_init();
    setup_uart_pins();
    setup_SPI(); 
    setup_sample_timer();

    status = PURPLE;
    delay_ms(1000);

    setup_sample_timer();

    ADC_conv.mode(OUTPUT);
    ADC_conv.type(PUSH_PULL);

    GPIO_pin display_trigger(PORTA, 1);
    display_trigger.mode(OUTPUT);
    display_trigger.clear();

    status = YELLOW; // general setup complete

    DAC.mode_normal(CH1, false, false);
    DAC.enable(CH1);

    FFT FFT;

    status = BLUE; // FFT coefficients computed

    FFT.setup(samples, NUM_SAMPLES);

    status = CYAN; // sample indicies shuffled

    sample_timer.clear_update_interrupt_flag();
    NVIC_ENABLE_INTERRUPT(54); // enable timer 6 global interrupt vector

    while (true)
    {
        if (display_flag)
        {
            status = YELLOW;
            display_trigger.toggle(); // set trigger before computing FFT
            results = FFT.compute_FFT();
            display_trigger.toggle(); // clear trigger after computing FFT
            //display the samples
            for (unsigned int f = 0; f < NUM_SAMPLES; f ++)
            {
                //DAC.registers->CH1_data_reg_12bit_right_allign = (uint16_t)((samples[f] * 100) + 100);
                DAC.registers->CH1_data_reg_12bit_right_allign = (uint16_t)((results[f] * 100) + 1);
            }
            display_flag = false;
            NVIC_ENABLE_INTERRUPT(54); // enable timer 6 global interrupt vector
            status = GREEN;
        }
    }
}

void ISR_timer6_global(void)
{
    sample_timer.clear_update_interrupt_flag(); // don't forget to clear the interrupt flag when done!
    if (sample_idx < NUM_SAMPLES)
    {
        
        ADC_conv.set();
        ADC_conv.clear();
        SPI.registers->data_reg = 0x0000;
        uint16_t SPI_data = SPI.registers->data_reg;
        int value = static_cast<int>(SPI_data) - (1U << 13) - 1; // center the 14-bit value around zero
        samples[sample_idx] = (static_cast<float>(value) / ((1U << 13) - 1)); // scale the data between -1.0 and 1.0
        sample_idx ++;
    }
    else
    {
        sample_idx = 0;
        display_flag = true;
        NVIC_DISABLE_INTERRUPT(54); // disable timer 6 global interrupt vector
    }
}