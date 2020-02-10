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


uint8_t reliabilityResults[TESTBED_MAX_SEQUENCE] = { 0 };
extern volatile uint32_t packets_received;
extern volatile uint32_t packets_transmitted;
extern volatile uint32_t packets_forwarded;
extern volatile uint32_t crc_fail_count;

void transmit_results()
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
    if (!p || !crc_ok || p->flags & TMDMA_CAT_FRAME_FLAGS_ADVERT || p->length == (TDMA_CAT_HEADER_SIZE - 1))
        return;

    ReliabilityTestPacket* pkt = (ReliabilityTestPacket*)p->payload;
    reliabilityResults[pkt->seq] |= 1 << (p->ttl + correction);
}

extern uint32_t serial_number;

#include "TDMACAT.h"
#define PREALLOC_SLOT_SIZE 12
uint8_t slots[PREALLOC_SLOT_SIZE] = { 8, 12, 15, 20, 23, 27, 30, 35, 38, 40, 44, 48};

void pre_allocate_tdma()
{
    TDMACATSlot adv;

    adv.device_identifier = 0;
    adv.slot_identifier = TDMA_CAT_ADVERTISEMENT_SLOT;
    adv.expiration = TDMA_CAT_NEVER_EXPIRE;
    adv.distance = 0;
    adv.flags = TDMA_SLOT_FLAGS_OWNER;

    for (int i = 0 ; i < 12; i++)
    {
        adv.slot_identifier = slots[i];
        tdma_set_slot(adv, false);
    }
}
int main()
{
    uBit.init();
    pre_allocate_tdma();
    uBit.radio.setTestRole(Transmitter);
    uBit.radio.enable();
    test_gpio_init();

    uBit.display.print((int)serial_number);
    uBit.sleep(500);

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
            while(uBit.radio.send((uint8_t*)&i, 4) != MICROBIT_OK);
            uBit.display.print(' ');
            wait_ms(10);
        }
    }
}

#endif