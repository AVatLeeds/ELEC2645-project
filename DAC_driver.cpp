#include <stdint.h>
#include "system.h"
#include "DAC_driver.h"

DAC_driver::DAC_driver()
{
    registers = (struct DAC_registers *)DAC_BASE;
    RCC_APB1ENR1 |= (1U << 29);
}

void DAC_driver::calibration_enable(uint8_t chan, bool state)
{
    uint8_t bit_location = 14 << ((chan & 0b1) << 4);
    state ? (registers->control_reg |= (1U << bit_location)) : (registers->control_reg &= ~(1U << bit_location));
}

void DAC_driver::DMA_interrupt_enable(uint8_t chan, bool state)
{
    uint8_t bit_location = 13 << ((chan & 0b1) << 4);
    state ? (registers->control_reg |= (1U << bit_location)) : (registers->control_reg &= ~(1U << bit_location));
}

void DAC_driver::DMA_enable(uint8_t chan, bool state)
{
    uint8_t bit_location = 12 << ((chan & 0b1) << 4);
    state ? (registers->control_reg |= (1U << bit_location)) : (registers->control_reg &= ~(1U << bit_location));
}

void DAC_driver::mask_amplitude(uint8_t chan, uint8_t value)
{
    uint8_t bit_location = 8 << ((chan & 0b1) << 4);
    value = (value & 0b1111) << bit_location;
    uint32_t reg = registers->control_reg;
    registers->control_reg = ((reg & (0b1111 << bit_location)) ^ value) ^ reg;
}

void DAC_driver::wave_select(uint8_t chan, uint8_t value)
{
    uint8_t bit_location = 6 << ((chan & 0b1) << 4);
    value = (value & 0b11) << bit_location;
    uint32_t reg = registers->control_reg;
    registers->control_reg = ((reg & (0b11 << bit_location)) ^ value) ^ reg;
}

void DAC_driver::trigger_select(uint8_t chan, uint8_t value)
{
    uint8_t bit_location = 3 << ((chan & 0b1) << 4);
    value = (value & 0b111) << bit_location;
    uint32_t reg = registers->control_reg;
    registers->control_reg = ((reg & (0b111 << bit_location)) ^ value) ^ reg;
}

void DAC_driver::trigger_enabled(uint8_t chan, bool state)
{
    uint8_t bit_location = 2 << ((chan & 0b1) << 4);
    state ? (registers->control_reg |= (1U << bit_location)) : (registers->control_reg &= ~(1U << bit_location));
}

void DAC_driver::enable(uint8_t chan)
{
    registers->control_reg |= (1U << ((chan & 0b1) << 4));
}

void DAC_driver::disable(uint8_t chan)
{
    registers->control_reg |= ~(1U << ((chan & 0b1) << 4));
}



void DAC_driver::trigger_channel_1()
{
    registers->software_trigger_reg = (1U << 0);
}

void DAC_driver::trigger_channel_2()
{
    registers->software_trigger_reg = (1U << 1);
}



void DAC_driver::write(uint8_t alignment, uint8_t chan, uint16_t value)
{
    switch (chan & 0b1)
    {
        case 0:
        switch (alignment)
        {
            case 0:
            registers->CH1_data_reg_12bit_right_allign = value;
            break;

            case 1:
            registers->CH1_data_reg_12bit_left_align = value;
            break;

            case 2:
            registers->CH1_data_reg_8bit = (value & 0xFF);
            break;
        }
        break;

        case 1:
        switch (alignment)
        {
            case 0:
            registers->CH2_data_reg_12bit_right_allign = value;
            break;
            
            case 1:
            registers->CH2_data_reg_12bit_left_align = value;
            break;

            case 2:
            registers->CH2_data_reg_8bit = (value & 0xFF);
            break;
        }
        break;
    }
}

void DAC_driver::dual_write(uint8_t alignment, uint16_t value)
{
    switch (alignment)
    {
        case 0:
        registers->dual_data_reg_12bit_right_align = value;
        break;

        case 1:
        registers->dual_data_reg_12bit_left_align = value;
        break;

        case 2:
        registers->dual_data_reg_8bit = (value & 0xFFFF);
        break;
    }
}

uint16_t DAC_driver::present_output(uint8_t chan)
{
    switch (chan & 0b1)
    {
        case 0:
        return registers->CH1_present_output_reg;
        break;

        case 1:
        return registers->CH2_present_output_reg;
        break;
    }
    return 0;
}



bool DAC_driver::busy_flag(uint8_t chan)
{
    uint8_t bit_location = 15 << ((chan & 0b1) << 4);
    return (registers->status_Reg >> bit_location) != 0U;
}

bool DAC_driver::cal_flag(uint8_t chan)
{
    uint8_t bit_location = 14 << ((chan & 0b1) << 4);
    return (registers->status_Reg >> bit_location) != 0U;
}

bool DAC_driver::DMA_underrun_flag(uint8_t chan)
{
    uint8_t bit_location = 13 << ((chan & 0b1) << 4);
    return (registers->status_Reg >> bit_location) != 0U;
}



void DAC_driver::cal_offset_trim(uint8_t chan, uint8_t value)
{
    registers->calibration_control_reg = ((value & 0b11111) << ((chan & 0b1) << 4));
}



void DAC_driver::mode_normal(uint8_t chan, bool internal, bool buffered)
{
    uint8_t channel_offset = ((chan & 0b1) << 4);
    registers->mode_control_reg = (static_cast<int>(!buffered) << (channel_offset + 1)) | (internal << channel_offset);
}

void DAC_driver::mode_sample_and_hold(uint8_t chan, bool internal, bool buffered)
{
    uint8_t channel_offset = ((chan & 0b1) << 4);
    registers->mode_control_reg = 0b100 | (static_cast<int>(!buffered) << (channel_offset + 1)) | (internal << channel_offset);
}