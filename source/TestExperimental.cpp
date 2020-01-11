/*
The MIT License (MIT)

Copyright (c) 2016 British Broadcasting Corporation.
This software is provided by Lancaster University by arrangement with the BBC.

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include "TestConfig.h"

#if CURRENT_TEST_CONFIG == TEST_CONFIG_EXPERIMENTAL

#pragma message "COMPILING SOURCE"

#include "MicroBit.h"

// #define USE_UBIT

MicroBit uBit;

void set_transmission_reception_gpio(int val)
{
#ifdef USE_UBIT
    uBit.io.P0.setDigitalValue(val);
#else
    if (val)
        NRF_GPIO->OUTSET |= 0x8;
    else
        NRF_GPIO->OUTCLR |= 0x8;
#endif
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


struct ReliabilityTestPacket
{
    uint16_t device_id;
    uint16_t start;
    uint32_t seq;
}__attribute__((packed));

uint8_t reliabilityResults[TEST_SOURCE_COUNT][TEST_PACKET_COUNT] = { 0 };
uint8_t previous_seq[TEST_SOURCE_COUNT] = { 0 };

uint8_t packet_missed_log[TEST_PACKET_COUNT] = {0};

extern uint32_t crc_fail_count;
// extern uint32_t radio_status;
extern uint32_t retransmit_count;



void log_num(int num)
{
#ifdef USE_UBIT
    uBit.serial.printf("%d",num);
#endif
}

int p0_state = 0;
volatile int start = 0;
extern volatile uint32_t packets_received;
extern volatile uint32_t packets_transmitted;
extern volatile uint32_t packets_forwarded;
void process_packet(PeridoFrameBuffer* p)
{
    if (!p)
        return;

    ReliabilityTestPacket* pkt = (ReliabilityTestPacket*)p->payload;
    if (pkt->start)
    {
        start = 1;
        return;
    }

    reliabilityResults[pkt->device_id][pkt->seq] = 1;

    previous_seq[pkt->device_id] = pkt->seq;
}

void packet_missed(PeridoFrameBuffer *p)
{
    ReliabilityTestPacket* pkt = (ReliabilityTestPacket*)p->payload;
    packet_missed_log[pkt->seq] = 1;
}

void increment_counter(int)
{
}

void log_string(const char * s)
{
#ifdef USE_UBIT
    uBit.serial.printf("%s\r\n",s);
#endif
}

ReliabilityTestPacket t;

extern void manual_poke();

#ifdef USE_UBIT

int main()
{
    uBit.init();

    // __disable_irq();

    // NRF_GPIO->DIR |= 0x8;

    // volatile int i = 500;
    // while(1)
    // {
    //     NRF_GPIO->OUTSET |= 0x8;
    //     i = 40;
    //     while(i-- > 0);
    //     NRF_GPIO->OUTCLR |= 0x8;
    //     i = 40;
    //     while(i-- > 0);
    // }

    uBit.radio.enable();
    t.device_id = TEST_SOURCE_ID;

    uBit.serial.printf("\r\nDevice id: %d ", t.device_id);

    set_rx_gpio(0);
    set_tx_enable_gpio(0);
    set_transmit_gpio(0);
    set_fwd_gpio(0);
    set_crc_fail_gpio(0);
    set_store_disable_gpio(0);
    set_tx_callback_gpio(0);
    set_wake_gpio(0);

    uint32_t blink_count = 0;
    uint32_t blink_state = 1;

    uBit.display.print('.');

    start = 0;
    t.seq = 100;
    int poke = 0;

    uBit.radio.send((uint8_t*)&t, sizeof(ReliabilityTestPacket), 0);

    while(1)
    {
// #define REPEAT
#ifdef REPEAT
        uBit.display.print('.');
        wait_ms(500);
        uBit.serial.printf("tx %d fwd %d rx %d crce: %d ", packets_transmitted, packets_forwarded, packets_received,crc_fail_count);
        uBit.display.print(' ');
        wait_ms(500);
#else
        while (!buttonA.isPressed());

        for (int i = 0; i < 1000; i++)
        {
            uBit.display.print('.');
            wait_ms(10);
            manual_poke();
            uBit.display.print(' ');
            wait_ms(10);
        }

        uBit.serial.printf("tx %d fwd %d rx %d crce: %d ", packets_transmitted, packets_forwarded, packets_received,crc_fail_count);

#endif
    }
}

#else //useubit

MicroBitSerial serial(USBTX, USBRX);
NRF51Timer timer0(NRF_TIMER0, TIMER0_IRQn, 4);
MicroBitPeridoRadio radio(timer0);
MicroBitButton buttonA(MICROBIT_PIN_BUTTON_A, MICROBIT_ID_BUTTON_A);

int main()
{
    // __disable_irq();

    NRF_GPIO->DIR |= 0x8;

    // volatile int i = 500;
    // while(1)
    // {
    //     NRF_GPIO->OUTSET |= 0x8;
    //     i = 40;
    //     while(i-- > 0);
    //     NRF_GPIO->OUTCLR |= 0x8;
    //     i = 40;
    //     while(i-- > 0);
    // }

    radio.enable();
    t.device_id = TEST_SOURCE_ID;

    serial.printf("\r\nDevice id: %d ", t.device_id);

    set_rx_gpio(0);
    set_tx_enable_gpio(0);
    set_transmit_gpio(0);
    set_fwd_gpio(0);
    set_crc_fail_gpio(0);
    set_store_disable_gpio(0);
    set_tx_callback_gpio(0);
    set_wake_gpio(0);

    uint32_t blink_count = 0;
    uint32_t blink_state = 1;

    start = 0;
    t.seq = 100;
    int poke = 0;

    radio.send((uint8_t*)&t, sizeof(ReliabilityTestPacket), 0);

    while(1)
    {
#define REPEAT
#ifdef REPEAT
        wait_ms(500);
        serial.printf("tx %d fwd %d rx %d crce: %d ", packets_transmitted, packets_forwarded, packets_received,crc_fail_count);
        wait_ms(500);
#else
        while (!buttonA.isPressed());

        for (int i = 0; i < 1000; i++)
        {
            wait_ms(10);
            manual_poke();
            wait_ms(10);
        }

        serial.printf("tx %d fwd %d rx %d crce: %d ", packets_transmitted, packets_forwarded, packets_received,crc_fail_count);

#endif
    }
}
#endif
#endif