#ifndef TIMERS_H
#define TIMERS_H

#include <stdint.h>

#define TIMER_1_ORIGIN  0x40012C00
#define TIMER_2_ORIGIN  0x40000000
#define TIMER_3_ORIGIN  0x40000400
#define TIMER_4_ORIGIN  0x40000800
#define TIMER_5_ORIGIN  0x40000C00
#define TIMER_6_ORIGIN  0x40001000
#define TIMER_7_ORIGIN  0x40001400
#define TIMER_8_ORIGIN  0x40013400
#define TIMER_15_ORIGIN 0x40014000
#define TIMER_16_ORIGIN 0x40014400
#define TIMER_17_ORIGIN 0x40014800

#define TIMER6  0
#define TIMER7  1

struct basic_timer
{
    volatile uint32_t control_reg_1;
    volatile uint32_t control_reg_2; 
    uint32_t :32;
    volatile uint32_t DMA_and_interrupt_enable_reg;
    volatile uint32_t status_reg;
    volatile uint32_t event_generation_reg;
    uint32_t :32;
    uint32_t :32;
    uint32_t :32;
    volatile uint32_t counter_reg;
    volatile uint32_t prescaler_reg;
    volatile uint32_t auto_reload_reg;
};

struct general_purpose_timer
{
    volatile uint32_t control_reg_1;
    volatile uint32_t control_reg_2;
    volatile uint32_t slave_mode_control_reg;
    volatile uint32_t DMA_and_interrupt_enable_reg;
    volatile uint32_t status_reg;
    volatile uint32_t event_generation_reg;
    volatile uint32_t capture_compare_mode_reg_1;
    volatile uint32_t capture_compare_mode_reg_2;
    volatile uint32_t capture_compare_enable_reg;
    volatile uint32_t counter_reg;
    volatile uint32_t prescaler_reg;
    volatile uint32_t auto_reload_reg;
    volatile uint32_t capture_compare_reg_1;
    volatile uint32_t capture_compare_reg_2;
    volatile uint32_t capture_compare_reg_3;
    volatile uint32_t capture_compare_reg_4;
    volatile uint32_t DMA_control_reg;
    volatile uint32_t DMA_address_reg;
    volatile uint32_t timer_2_option_reg_1;
    volatile uint32_t timer_3_option_reg_1;
    volatile uint32_t timer_2_option_reg_2;
    volatile uint32_t timer_3_option_reg_2;
};

class Basic_timer
{
    public:

    Basic_timer(uint8_t which);

    volatile struct basic_timer * registers;

    void remap_update_interrupt_flag_to_count(bool state);
    void threshold_reg_buffered(bool state);
    void one_pulse_mode(bool state);
    void only_update_on_verflow(bool state);
    void update_disabled(bool state);
    void start();
    void stop();

    void master_mode_reset();
    void master_mode_enable();
    void master_mode_update();

    void DMA_request_on_update(bool state);
    void interrupt_on_update(bool state);

    bool get_update_interrupt_flag();
    void clear_update_interrupt_flag();

    void generate_update();

    uint32_t count();

    void set_prescaler(uint16_t prescaler);
    void set_threshold(uint16_t threshold);
};

#endif