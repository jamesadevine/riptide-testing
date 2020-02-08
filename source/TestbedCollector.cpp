#include "TestConfig.h"
#include "GPIODebug.h"

#if CURRENT_TEST_CONFIG == TEST_CONFIG_TESTBED_COLLECTOR

#pragma message "COMPILING COLLECTOR"

#include "MicroBit.h"

MicroBit uBit;

struct ReliabilityTestPacket
{
    uint32_t seq;
}__attribute__((packed));

extern volatile uint32_t packets_received;
extern volatile uint32_t packets_transmitted;
extern volatile uint32_t packets_forwarded;
extern volatile uint32_t crc_fail_count;

void process_packet(PeridoFrameBuffer* p, bool crc_ok, int)
{
    if (!p)
        return;

    if (!crc_ok)
    {
        uBit.serial.printf("CRC ERROR");
        return;
    }

    uBit.serial.printf("sequence number, p1, p2\r\n");

    uint8_t iterator = 0;

    uint16_t* seq_start = (uint16_t *)p->payload;
    uint16_t seq_counter = *seq_start;
    iterator = 2;

    while (iterator < TESTBED_MAX_PAYLOAD + 2 && seq_counter < TESTBED_MAX_SEQUENCE)
    {
        uint8_t byte = p->payload[iterator++];
        uBit.serial.printf("%d, %d, %d\r\n", seq_counter, (byte & 0x01) ? 1 : 0, (byte & 0x02) ? 1 : 0);
        uBit.serial.printf("%d, %d, %d\r\n", seq_counter + 1 , (byte & 0x04) ? 1 : 0, (byte & 0x08) ? 1 : 0);
        uBit.serial.printf("%d, %d, %d\r\n", seq_counter + 2 , (byte & 0x10) ? 1 : 0, (byte & 0x20) ? 1 : 0);
        uBit.serial.printf("%d, %d, %d\r\n", seq_counter + 3 , (byte & 0x40) ? 1 : 0, (byte & 0x80) ? 1 : 0);
        seq_counter += 4;
    }
}

int main()
{
    uBit.init();
    uBit.radio.setTestRole(Collector);
    uBit.radio.enable();

    test_gpio_init();

    while(1)
    {
        uBit.display.print('C');
        uBit.sleep(500);
        uBit.display.print(' ');
        uBit.sleep(500);
    }
}

#endif