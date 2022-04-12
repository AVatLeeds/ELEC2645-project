#ifndef LCD_HD61830_DRIVER_H
#define LCD_HD61830_DRIVER_H

#include <stdint.h>

#define MODE_CONTROL_REG    0x0
#define CHAR_PITCH_REG      0x1
#define NUM_CHARS_REG       0x2
#define CURSOR_POS_REG      0x4
#define DISP_START_LOW_REG  0x8
#define DISP_START_HIGH_REG 0x9
#define CURSOR_POS_LOW_REG  0xA
#define CURSOR_POS_HIGH_REG 0xB
#define DISP_DATA_WRITE_REG 0xC
#define DISP_DATA_READ_REG  0dD
#define BIT_CLEAR_REG       0xE
#define BIT_SET_REG         0xF

/*
*/

class pin_driver
{
    public:
    
    //pin_driver();
    //virtual ~pin_driver();
    virtual void E_pin(bool state);
    virtual void RW_pin(bool state);
    virtual void RS_pin(bool state);
    virtual void write_data(uint8_t data);
    virtual uint8_t read_data();
    virtual void delay();
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