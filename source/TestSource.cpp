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

#if CURRENT_TEST_CONFIG == TEST_CONFIG_SOURCE

#pragma message "COMPILING SOURCE"

#include "MicroBit.h"

MicroBit uBit;

void set_transmission_reception_gpio(int val)
{
    uBit.io.P0.setDigitalValue(val);
}

void set_user_placed_gpio(int val)
{
    uBit.io.P8.setDigitalValue(val);
}

void set_rx_gpio(int val)
{
    uBit.io.P1.setDigitalValue(val);
}
void set_tx_enable_gpio(int val)
{
    // uBit.io.P1.setDigitalValue(val);
}
void set_transmit_gpio(int val)
{
    uBit.io.P2.setDigitalValue(val);
}
void set_fwd_gpio(int val)
{
    // uBit.io.P8.setDigitalValue(val);
}
void set_crc_fail_gpio(int val)
{
    uBit.io.P12.setDigitalValue(val);
}
void set_store_disable_gpio(int val)
{
    uBit.io.P13.setDigitalValue(val);
}
void set_tx_callback_gpio(int val)
{
    uBit.io.P14.setDigitalValue(val);
}
void set_wake_gpio(int val)
{
    uBit.io.P15.setDigitalValue(val);
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
extern uint32_t radio_status;
extern uint32_t retransmit_count;

void log_num(int num)
{
    uBit.serial.printf("%d",num);
}

int p0_state = 0;
volatile int start = 0;
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
    uBit.serial.printf("%s\r\n",s);
}

ReliabilityTestPacket t;

int main()
{
    uBit.init();
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

    while(1)
    {
        start = 0;
        t.seq = 0;
        crc_fail_count = 0;
        memset(reliabilityResults, 0, TEST_SOURCE_COUNT * TEST_PACKET_COUNT);
        memset(packet_missed_log, 0, TEST_PACKET_COUNT);
        log_string(" waiting to start...");
        while (1)
        {
            if (uBit.buttonA.isPressed() || start)
                break;
        }

        if (!start)
        {
            log_string("Send start!");
            t.start = 1;
            uBit.radio.send((uint8_t*)&t, sizeof(ReliabilityTestPacket), 0);
            t.start = 0;
            while(!start);
            // wait_ms(100);
        }

        uint64_t tstart = system_timer_current_time_us();


        log_string("Go!");

        while (t.seq < TEST_PACKET_COUNT)
        {
            int ret = uBit.radio.send((uint8_t*)&t, sizeof(ReliabilityTestPacket), 0);

            if (ret == MICROBIT_OK)
                t.seq++;

            blink_count++;

            if (blink_count == 0xfff)
            {
                if (blink_state)
                    uBit.display.print(' ');
                else
                    uBit.display.print('.');

                blink_count = 0;
                blink_state = !blink_state;
            }

            // wait_ms(WAIT_BETWEEN_PACKETS);
            // uBit.serial.printf("RS: %d, HWDS: %d ", radio_status, NRF_RADIO->STATE);
        }

        uint64_t tend = system_timer_current_time_us();

        log_string("complete!\r\n");

        // wait for others to complete.
        wait_ms(1000);

        for(int i = 0; i < TEST_SOURCE_COUNT; i++)
        {
            uBit.serial.printf("\r\nDevice %d missed: ", i);
            for (int j = 0; j < TEST_PACKET_COUNT; j++)
            {
                if (reliabilityResults[i][j] == 0)
                    uBit.serial.printf("%d, ",j);
            }
        }

        uBit.serial.printf("\r\nThis device thinks it missed: ");

        for (int j = 0; j < TEST_PACKET_COUNT; j++)
        {
            if (packet_missed_log[j] == 1)
                uBit.serial.printf("%d, ",j);
        }

        uBit.serial.printf("\r\nCRC fails: %d", crc_fail_count);
        uBit.serial.printf("\r\nretransmit count: %d", retransmit_count);
        uBit.serial.printf("\r\nTime: %d", (uint32_t)((tend - tstart) / 1000));

    }
}

#endif
