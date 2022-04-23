#include <stdint.h>
#include "system.h"
#include "SPI_driver.h"

SPI_driver::SPI_driver(uint8_t which)
{
    switch (which) {
        case SPI1:
        registers = (struct SPI_registers *)SPI_1_ORIGIN;
        RCC_APB2ENR |= (1U << 12); // enable clock to SPI1 peripheral
        break;

        case SPI2:
        registers = (struct SPI_registers *)SPI_2_ORIGIN;
        RCC_APB1ENR1 |= (1U << 14);
        break;

        case SPI3:
        registers = (struct SPI_registers *)SPI_3_ORIGIN;
        RCC_APB1ENR1 |= (1U << 15);
        break;

        default:
        registers = (struct SPI_registers *)SPI_1_ORIGIN;
        RCC_APB2ENR |= (1U << 12); // enable clock to SPI1 peripheral
    }

    
}

// methods for setup of control register 1

void SPI_driver::bidirectional_mode(bool state)
{
    state ? (registers->control_reg_1 |= (1U << 15)) : (registers->control_reg_1 &= ~(1U << 15));
}

void SPI_driver::bidirectional_rx_only()
{
    registers->control_reg_1 &= ~(1U << 14);
}

void SPI_driver::bidirectional_tx_only()
{
    registers->control_reg_1 |= (1U << 14);
}

void SPI_driver::CRC_enabled(bool state)
{
    state ? (registers->control_reg_1 |= (1U << 13)) : (registers->control_reg_1 &= ~(1U << 13));
}

void SPI_driver::transmit_CRC_next()
{
    registers->control_reg_1 |= (1U << 12);
}

void SPI_driver::CRC_8bit()
{
    registers->control_reg_1 &= ~(1U << 11);
}

void SPI_driver::CRC_16bit()
{
    registers->control_reg_1 |= (1U << 11);
}

void SPI_driver::rx_only_mode(bool state)
{
    state ? (registers->control_reg_1 |= (1U << 10)) : (registers->control_reg_1 &= ~(1U << 10));
}

void SPI_driver::software_slave_management(bool state)
{
    state ? (registers->control_reg_1 |= (1U << 9)) : (registers->control_reg_1 &= ~(1U << 9));
}

void SPI_driver::internal_slave_select(bool state)
{
    state ? (registers->control_reg_1 |= (1U << 8)) : (registers->control_reg_1 &= ~(1U << 8));
}

void SPI_driver::frame_MSB_first()
{
    registers->control_reg_1 &= ~(1U << 7);
}

void SPI_driver::frame_LSB_first()
{
    registers->control_reg_1 |= (1U << 7);
}

void SPI_driver::enable()
{
    registers->control_reg_1 |= (1U << 6);
}

void SPI_driver::disable()
{
    registers->control_reg_1 &= ~(1U << 6);
}

void SPI_driver::baud_rate_divisor(uint8_t divisor)
{
    divisor = (divisor & 0b111) << 3;
    uint32_t reg = registers->control_reg_1;
    registers->control_reg_1 = ((reg & (0b111 << 3)) ^ divisor) ^ reg;
}

void SPI_driver::master_mode()
{
    registers->control_reg_1 |= (1U << 2);
}

void SPI_driver::slave_mode()
{
    registers->control_reg_1 &= ~(1U << 2);
}

void SPI_driver::clock_polarity(bool polarity)
{
    polarity ? (registers->control_reg_1 |= (1U << 1)) : (registers->control_reg_1 &= ~(1U << 1));
}

void SPI_driver::clock_phase(bool phase)
{
    phase ? (registers->control_reg_1 |= (1U << 0)) : (registers->control_reg_1 &= ~(1U << 0));
}

// methods for setup of control register 2

void SPI_driver::FIFO_threshold_8bit()
{
    registers->control_reg_2 |= (1U << 12);
}

void SPI_driver::FIFO_threshold_16bit()
{
    registers->control_reg_2 &= ~(1U << 12);
}

void SPI_driver::data_size(uint32_t size)
{
    size = ((size - 1) & 0b1111) << 8;
    uint32_t reg = registers->control_reg_2;
    registers->control_reg_2 = ((reg & (0b1111 << 8)) ^ size) ^ reg;
}

void SPI_driver::tx_empty_interrupt_enable(bool state)
{
    state ? (registers->control_reg_2 |= (1U << 7)) : (registers->control_reg_2 &= ~(1U << 7));
}

void SPI_driver::rx_not_empty_interrupt_enable(bool state)
{
    state ? (registers->control_reg_2 |= (1U << 6)) : (registers->control_reg_2 &= ~(1U << 6));
}

void SPI_driver::error_interrupt_enable(bool state)
{
    state ? (registers->control_reg_2 |= (1U << 5)) : (registers->control_reg_2 &= ~(1U << 5));
}

void SPI_driver::frame_format_Motorola()
{
    registers->control_reg_2 &= ~(1U << 4);
}

void SPI_driver::frame_format_TI()
{
    registers->control_reg_2 |= (1U << 4);
}

void SPI_driver::NSS_pulse(bool state)
{
    state ? (registers->control_reg_2 |= (1U << 3)) : (registers->control_reg_2 &= ~(1U << 3));
}

void SPI_driver::SS_output_enable(bool state)
{
    state ? (registers->control_reg_2 |= (1U << 2)) : (registers->control_reg_2 &= ~(1U << 2));
}

// methods for status register

uint8_t SPI_driver::FIFO_tx_level()
{
    return (registers->status_reg >> 11) & 0b11;
}

uint8_t SPI_driver::FIFO_rx_level()
{
    return (registers->status_reg >> 9) & 0b11;
}

bool SPI_driver::frame_error()
{
    return (registers->status_reg & (1U << 8)) != 0U;
}

bool SPI_driver::busy_flag()
{
    return (registers->status_reg & (1U << 7)) != 0U;
}

bool SPI_driver::overrun_flag()
{
    return (registers->status_reg & (1U << 6)) != 0U;
}

bool SPI_driver::mode_fault()
{
    return (registers->status_reg & (1U << 5)) != 0U;
}

bool SPI_driver::CRC_error()
{
    return (registers->status_reg & (1U << 4)) != 0U;
}

bool SPI_driver::tx_buffer_empty()
{
    return (registers->status_reg & (1U << 1)) != 0U;
}

bool SPI_driver::rx_buffer_not_empty()
{
    return (registers->status_reg & (1U << 0)) != 0U;
}