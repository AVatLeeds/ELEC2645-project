#ifndef DAC_DRIVER_H
#define DAC_DRIVER_H

#include <stdint.h>

#define DAC_BASE    0x40007400

#define CH1 0
#define CH2 1

#define NO_WAVE     0
#define NOISE       1
#define TRIANGLE    2

#define TIM6_TRGO   0
#define TIM8_TRGO   1
#define TIM7_TRGO   2
#define TIM5_TRGO   3
#define TIM2_TRGO   4
#define TIM4_TRGO   5
#define EXTI9       6
#define SWTRIG      7

#define ALIGN_RIGHT 0
#define ALIGN_LEFT  1
#define ALIGN_8BIT  2

struct DAC_registers
{
    volatile uint32_t control_reg;
    volatile uint32_t software_trigger_reg;
    volatile uint32_t CH1_data_reg_12bit_right_allign;
    volatile uint32_t CH1_data_reg_12bit_left_align;
    volatile uint32_t CH1_data_reg_8bit;
    volatile uint32_t CH2_data_reg_12bit_right_allign;
    volatile uint32_t CH2_data_reg_12bit_left_align;
    volatile uint32_t CH2_data_reg_8bit;
    volatile uint32_t dual_data_reg_12bit_right_align;
    volatile uint32_t dual_data_reg_12bit_left_align;
    volatile uint32_t dual_data_reg_8bit;
    volatile uint32_t CH1_present_output_reg;
    volatile uint32_t CH2_present_output_reg;
    volatile uint32_t status_Reg;
    volatile uint32_t calibration_control_reg;
    volatile uint32_t mode_control_reg;
    volatile uint32_t CH1_sample_timer_reg;
    volatile uint32_t CH2_sample_timer_reg;
    volatile uint32_t sample_and_hold_reg;
    volatile uint32_t sample_and_hold_refresh_time_reg;
};

class DAC_driver
{
    public:

    DAC_driver();

    volatile struct DAC_registers * registers;

    void calibration_enable(uint8_t chan, bool state);
    void DMA_interrupt_enable(uint8_t chan, bool state);
    void DMA_enable(uint8_t chan, bool state);
    void mask_amplitude(uint8_t chan, uint8_t value);
    void wave_select(uint8_t chan, uint8_t value);
    void trigger_select(uint8_t chan, uint8_t value);
    void trigger_enabled(uint8_t chan, bool state);
    void enable(uint8_t chan);
    void disable(uint8_t chan);

    void trigger_channel_1();
    void trigger_channel_2();

    void write(uint8_t alignment, uint8_t chan, uint16_t value);
    void dual_write(uint8_t alignment, uint16_t value);
    uint16_t present_output(uint8_t chan);

    bool busy_flag(uint8_t chan);
    bool cal_flag(uint8_t chan);
    bool DMA_underrun_flag(uint8_t chan);

    void cal_offset_trim(uint8_t chan, uint8_t value);

    void mode_normal(uint8_t chan, bool internal, bool buffered);
    void mode_sample_and_hold(uint8_t chan, bool internal, bool buffered);
};

#endif
