#include "TestConfig.h"

#if CURRENT_TEST_CONFIG == TEST_CONFIG_REPEATER

#pragma message "COMPILING REPEATER"

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

// MicroBitPin col1(100, MICROBIT_PIN_P3, PIN_CAPABILITY_STANDARD);
// MicroBitPin row1(101, ROW1, PIN_CAPABILITY_STANDARD);

int main()
{
    uBit.init();
    uBit.radio.enable();

    // col1.setDigitalValue(0);
    // row1.setDigitalValue(1);

    uBit.serial.printf("\r\nDevice id: %d ", t.device_id);

    set_rx_gpio(0);
    set_tx_enable_gpio(0);
    set_transmit_gpio(0);
    set_fwd_gpio(0);
    set_crc_fail_gpio(0);
    set_store_disable_gpio(0);
    set_tx_callback_gpio(0);
    set_wake_gpio(0);

    uBit.serial.printf("REPEATER");

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
        while (1)
        {
            if (uBit.buttonA.isPressed())
                break;

            blink_count++;

            if (blink_count == 0xffff)
            {
                if (blink_state)
                    uBit.display.print(' ');
                else
                    uBit.display.print('.');

                blink_count = 0;
                blink_state = !blink_state;
            }
        }

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

    }
}

#endif
