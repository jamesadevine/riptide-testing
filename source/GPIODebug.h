#ifndef GPIO_DEBUG_H
#define GPIO_DEBUG_H

void set_transmission_reception_gpio(int val);
void set_user_placed_gpio(int val);
void set_rx_gpio(int val);
void set_tx_enable_gpio(int val);
void set_transmit_gpio(int val);
void set_fwd_gpio(int val);
void set_crc_fail_gpio(int val);
void set_store_disable_gpio(int val);
void set_tx_callback_gpio(int val);
void set_wake_gpio(int val);
void test_gpio_init();



#endif