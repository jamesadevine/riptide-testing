#include "TestConfig.h"
#include "GPIODebug.h"

#if CURRENT_TEST_CONFIG == TEST_CONFIG_TESTBED_TRANSMITTER

#pragma message "COMPILING TRANSMITTER"

#include "MicroBit.h"

MicroBit uBit;

struct ReliabilityTestPacket
{
    uint32_t seq;
}__attribute__((packed));

uint8_t reliabilityResults[TESTBED_MAX_SEQUENCE][TESTBED_REPETITIONS_SEQUENCE] = { 0 };
extern volatile uint32_t packets_received;
extern volatile uint32_t packets_transmitted;
extern volatile uint32_t packets_forwarded;
extern volatile uint32_t crc_fail_count;

void process_packet(PeridoFrameBuffer* p, bool crc_ok, int)
{
    if (!p || !crc_ok)
        return;

    ReliabilityTestPacket* pkt = (ReliabilityTestPacket*)p->payload;
    reliabilityResults[pkt->seq][p->ttl + 1] = 1;
}

extern void manual_poke(PeridoFrameBuffer*);

PeridoFrameBuffer buf;


void transmit_results()
{
    uint8_t* data = (uint8_t*) malloc(255);
    memset(data, 0, 255);
    uint16_t* seq_start = (uint16_t *)data;

    uint8_t iterator = 2;
    uint16_t seq_counter = 0;

    int verified = 0;

    for (int i = 0; i < TESTBED_MAX_SEQUENCE; i++)
    {
        if (reliabilityResults[seq_counter][1] || reliabilityResults[seq_counter][0])
            verified++;
    }

    uBit.serial.printf("VERIFIED COUNT: %d", verified);

    uBit.radio.disable();

    while (seq_counter < TESTBED_MAX_SEQUENCE)
    {
        uBit.serial.printf("seq %d\r\n", seq_counter);
        *seq_start = seq_counter;
        iterator = 2;
        while (iterator < TESTBED_MAX_PAYLOAD + 2 && seq_counter < TESTBED_MAX_SEQUENCE)
        {
            uint8_t byte = 0;
            byte |= ((reliabilityResults[seq_counter][1] ? (0x1 << 1) : 0 ) | (reliabilityResults[seq_counter][0] ? (0x1 << 0) : 0));
            byte |= ((reliabilityResults[seq_counter + 1][1] ? (0x1 << 3) : 0 ) | (reliabilityResults[seq_counter + 1][0] ? (0x1 << 2) : 0));
            byte |= ((reliabilityResults[seq_counter + 2][1] ? (0x1 << 5) : 0 ) | (reliabilityResults[seq_counter + 2][0] ? (0x1 << 4) : 0));
            byte |= ((reliabilityResults[seq_counter + 3][1] ? (0x1 << 7) : 0 ) | (reliabilityResults[seq_counter + 3][0] ? (0x1 << 6) : 0));
            data[iterator++] = byte;
            seq_counter += 4;
        }

        uBit.serial.printf("tx seq %d, %d %d\r\n", *seq_start, data[1], data[0]);
        uBit.radio.sendTestResults(data, TESTBED_MAX_PAYLOAD + 2);
        wait_ms(1000);
    }

    free(data);

    uBit.radio.enable();
}

int main()
{
    uBit.init();
    uBit.radio.setTestRole(Transmitter);
    uBit.radio.enable();
    test_gpio_init();

    ReliabilityTestPacket* t = (ReliabilityTestPacket *)buf.payload;

    buf.id = microbit_random(65535);
    buf.length = 4 + MICROBIT_PERIDO_HEADER_SIZE - 1;
    buf.app_id = 0;
    buf.namespace_id = 0;
    buf.ttl = MICROBIT_PERIDO_DEFAULT_TTL;
    buf.initial_ttl = MICROBIT_PERIDO_DEFAULT_TTL;
    buf.time_since_wake = 0;
    buf.period = 0;

    while(1)
    {
        uBit.display.print('T');
        while (1)
        {
            if (uBit.buttonB.isPressed())
                break;

            if (uBit.buttonA.isPressed())
                transmit_results();
        }

        for (int i = 0; i < 1000; i++)
        {
            uBit.display.print('.');
            wait_ms(10);
            t->seq = i;
            manual_poke(&buf);
            uBit.display.print(' ');
            wait_ms(10);
        }
    }
}

#endif