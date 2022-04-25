#include <stdint.h>
#include "system.h"
#include "timers.h"

Basic_timer::Basic_timer(uint8_t which)
{
    switch (which) {
        case TIMER6:
        registers = (struct basic_timer *)TIMER_6_ORIGIN;
        RCC_APB1ENR1 |= (1U << 4);
        break;

        case TIMER7:
        registers = (struct basic_timer *)TIMER_7_ORIGIN;
        RCC_APB1ENR1 |= (1U << 5);
        break;
        
        default:
        registers = (struct basic_timer *)TIMER_6_ORIGIN;
        RCC_APB1ENR1 |= (1U << 4);
    }
}

void Basic_timer::remap_update_interrupt_flag_to_count(bool state)
{
    state ? (registers->control_reg_1 |= (1U << 11)) : (registers->control_reg_1 &= ~(1U << 11)); 
}

void Basic_timer::threshold_reg_buffered(bool state)
{
    state ? (registers->control_reg_1 |= (1U << 7)) : (registers->control_reg_1 &= ~(1U << 7));
}

void Basic_timer::one_pulse_mode(bool state)
{
    state ? (registers->control_reg_1 |= (1U << 3)) : (registers->control_reg_1 &= ~(1U << 3));
}

void Basic_timer::only_update_on_verflow(bool state)
{
    state ? (registers->control_reg_1 |= (1U << 2)) : (registers->control_reg_1 &= ~(1U << 2));
}

void Basic_timer::update_disabled(bool state)
{
    state ? (registers->control_reg_1 |= (1U << 1)) : (registers->control_reg_1 &= ~(1U << 1));
}

void Basic_timer::start()
{
    registers->control_reg_1 |= (1U << 0);
}

void Basic_timer::stop()
{
    registers->control_reg_1 &= ~(1U << 0);
}

void Basic_timer::master_mode_reset()
{
    uint32_t reg = registers->control_reg_2;
    registers->control_reg_2 = ((reg & (0b111 << 4)) ^ (0b000 << 4)) ^ reg;
}

void Basic_timer::master_mode_enable()
{
    uint32_t reg = registers->control_reg_2;
    registers->control_reg_2 = ((reg & (0b111 << 4)) ^ (0b001 << 4)) ^ reg;
}

void Basic_timer::master_mode_update()
{
    uint32_t reg = registers->control_reg_2;
    registers->control_reg_2 = ((reg & (0b111 << 4)) ^ (0b010 << 4)) ^ reg;
}

void Basic_timer::DMA_request_on_update(bool state)
{
    state ? (registers->DMA_and_interrupt_enable_reg |= (1U << 8)) : (registers->DMA_and_interrupt_enable_reg &= ~(1U << 8));
}

void Basic_timer::interrupt_on_update(bool state)
{
    state ? (registers->DMA_and_interrupt_enable_reg |= (1U << 0)) : (registers->DMA_and_interrupt_enable_reg &= ~(1U << 0));
}

bool Basic_timer::get_update_interrupt_flag()
{
    return (registers->status_reg & 1U) != 0U;
}

void Basic_timer::clear_update_interrupt_flag()
{
    registers->status_reg &= ~1U;
}

void Basic_timer::generate_update()
{
    registers->event_generation_reg |= 1U;
}

uint32_t Basic_timer::count()
{
    return registers->counter_reg;
}

void Basic_timer::set_prescaler(uint16_t prescaler)
{
    registers->prescaler_reg = prescaler;
}

void Basic_timer::set_threshold(uint16_t threshold)
{
    registers->auto_reload_reg = threshold;
}





