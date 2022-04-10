#ifndef LCD_HD61830_DRIVER_H
#define LCD_HD61830_DRIVER_H

#include <stdint.h>

/*
*/

class pin_driver
{
    public:
    
    void E_pin(bool state);
    void RW_pin(bool state);
    void RS_pin(bool state);
    void write_data(uint8_t data);
    uint8_t read_data();
    void delay();
};

class HD61830_driver
{
    public:

    HD61830_driver(pin_driver * pin_driver);

    void write_register(uint8_t address, uint8_t data);
    void write_display_data(uint8_t data_byte);
    void display_on();
    void display_off();
    void master_mode(); // sync signal for parallel operation is output
    void slave_mode(); // sync signal for parallel operation is input
    void set_character_mode();
    void set_graphics_mode();
    void cursor_on();
    void cursor_off();
    void character_blink();
    void cursor_blink();
    void internal_CG(); // use internal character generator ROM
    void external_CG(); // use external character generator ROM

    private:

    pin_driver * _pin_driver;
    // default mode is: display on, master mode, character mode, cursor blink, internal character generator
    uint8_t _mode = 0b00111100;

};

#endif