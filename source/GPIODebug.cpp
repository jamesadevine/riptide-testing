#include "GPIODebug.h"
#include "nrf.h"
#include "MicroBit.h"

#define PIN_P0_BIT_MSK      0x8

extern MicroBit uBit;

void set_transmission_reception_gpio(int val)
{
// #ifdef USE_UBIT
//     uBit.io.P0.setDigitalValue(val);
// #else
    if (val)
        NRF_GPIO->OUTSET |= PIN_P0_BIT_MSK;
    else
        NRF_GPIO->OUTCLR |= PIN_P0_BIT_MSK;
// #endif
}

void set_user_placed_gpio(int val)
{
    // uBit.io.P8.setDigitalValue(val);
}

void set_rx_gpio(int val)
{
    // uBit.io.P1.setDigitalValue(val);
}
void set_tx_enable_gpio(int val)
{
    // uBit.io.P1.setDigitalValue(val);
}
void set_transmit_gpio(int val)
{
    // uBit.io.P2.setDigitalValue(val);
}
void set_fwd_gpio(int val)
{
    // uBit.io.P8.setDigitalValue(val);
}
void set_crc_fail_gpio(int val)
{
    // uBit.io.P12.setDigitalValue(val);
}
void set_store_disable_gpio(int val)
{
    // uBit.io.P13.setDigitalValue(val);
}
void set_tx_callback_gpio(int val)
{
    // uBit.io.P14.setDigitalValue(val);
}
void set_wake_gpio(int val)
{
    // uBit.io.P15.setDigitalValue(val);
}

void test_gpio_init()
{
    NRF_GPIO->DIR |= PIN_P0_BIT_MSK;
    set_transmission_reception_gpio(0);
    set_user_placed_gpio(0);
    set_rx_gpio(0);
    set_tx_enable_gpio(0);
    set_transmit_gpio(0);
    set_fwd_gpio(0);
    set_crc_fail_gpio(0);
    set_store_disable_gpio(0);
    set_tx_callback_gpio(0);
    set_wake_gpio(0);
}