#include "TestConfig.h"
#include "GPIODebug.h"

#if CURRENT_TEST_CONFIG == TEST_CONFIG_TESTBED_OBSERVER

#pragma message "COMPILING OBSERVER"

#include "MicroBit.h"

MicroBit uBit;

struct ReliabilityTestPacket
{
    uint32_t seq;
}__attribute__((packed));

uint8_t reliabilityResults[TESTBED_MAX_SEQUENCE] = { 0 };
extern volatile uint32_t packets_received;
extern volatile uint32_t packets_transmitted;
extern volatile uint32_t packets_forwarded;
extern volatile uint32_t crc_fail_count;

void transmit_results(MicroBitEvent)
{
    uBit.radio.disable();

tx_results:
    uint8_t iterator = TESTBED_RESULT_HEADER_SIZE;
    uint16_t seq_counter = 0;

    uint8_t* data = (uint8_t*) malloc(255);
    memset(data, 0, 255);
    uint16_t* seq_start = (uint16_t *)data;

    while (seq_counter < TESTBED_MAX_SEQUENCE)
    {
        *seq_start = seq_counter;

        iterator = TESTBED_RESULT_HEADER_SIZE;
        while (iterator < TESTBED_MAX_PAYLOAD + TESTBED_RESULT_HEADER_SIZE && seq_counter < TESTBED_MAX_SEQUENCE)
            data[iterator++] = reliabilityResults[seq_counter++];

        uBit.radio.sendTestResults(data, TESTBED_MAX_PAYLOAD + TESTBED_RESULT_HEADER_SIZE);
        wait_ms(1000);
    }

    free(data);

    while (1)
    {
        uBit.display.print('X');
        while(!uBit.buttonA.isPressed());
        goto tx_results;
    }
}

void process_packet(TDMACATSuperFrame* p, bool crc_ok, int correction)
{
    if (!p || !crc_ok)
        return;

    ReliabilityTestPacket* pkt = (ReliabilityTestPacket*)p->payload;
    reliabilityResults[pkt->seq] |= 1 << (p->ttl + correction);
}

void clear_data(MicroBitEvent)
{
    memset(reliabilityResults, 0, TESTBED_MAX_SEQUENCE);
}

int main()
{
    uBit.init();
    uBit.radio.setTestRole(Observer);
    memset(reliabilityResults, 0, TESTBED_MAX_SEQUENCE);
    uBit.radio.enable();
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, transmit_results);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, clear_data);

    test_gpio_init();

    while(1)
    {
        uBit.display.print('O');
        uBit.sleep(500);
        // uBit.serial.printf("tx %d fwd %d rx %d crce: %d ", packets_transmitted, packets_forwarded, packets_received,crc_fail_count);
        uBit.display.print(' ');
        uBit.sleep(500);
    }
}

#endif