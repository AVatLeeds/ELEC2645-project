#ifndef SPI_H
#define SPI_H

#include <stdint.h>

#define SPI_1_ORIGIN  0x40013000
#define SPI_2_ORIGIN  0x40003800
#define SPI_3_ORIGIN  0x40003C00

#define SPI1    0
#define SPI2    1
#define SPI3    3

struct SPI_registers
{
    volatile uint32_t control_reg_1;
    volatile uint32_t control_reg_2;
    volatile uint32_t status_reg;
    volatile uint32_t data_reg;
    volatile uint32_t CRC_polynomial_reg;
    volatile uint32_t CRC_reg_RX;
    volatile uint32_t CRC_reg_TX;
};

class SPI_driver
{
    public:

    SPI_driver(uint8_t which);

    struct SPI_registers * registers;
    void bidirectional_mode(bool state);
    void bidirectional_rx_only();
    void bidirectional_tx_only();
    void CRC_enabled(bool state);
    void transmit_CRC_next();
    void CRC_8bit();
    void CRC_16bit();
    void rx_only_mode(bool state);
    void software_slave_management(bool state);
    void internal_slave_select(bool state);
    void frame_MSB_first();
    void frame_LSB_first();
    void enable();
    void disable();
    void baud_rate_divisor(uint8_t divisor);
    void master_mode();
    void slave_mode();
    void clock_polarity(bool polarity);
    void clock_phase(bool phase);

    // implement LDMA_TX
    // implement LDMA_RX
    void FIFO_threshold_8bit();
    void FIFO_threshold_16bit();
    void data_size(uint8_t size);
    void tx_empty_interrupt_enable(bool state);
    void rx_not_empty_interrupt_enable(bool state);
    void error_interrupt_enable(bool state);
    void frame_format_Motorola();
    void frame_format_TI();
    void NSS_pulse(bool state);
    void SS_output_enable(bool state);
    // implement tx buffer DMA
    // implement rx buffer DMA

    uint8_t FIFO_tx_level();
    uint8_t FIFO_rx_level();
    bool frame_error();
    bool busy_flag();
    bool overrun_flag();
    bool mode_fault();
    bool CRC_error();
    bool tx_buffer_empty();
    bool rx_buffer_not_empty();

    uint16_t transfer(uint16_t data);
    void transmit(uint16_t data);
    uint16_t receive();

};

#endif