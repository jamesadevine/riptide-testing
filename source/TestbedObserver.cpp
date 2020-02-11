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
    if (!p || !crc_ok || p->flags & TMDMA_CAT_FRAME_FLAGS_ADVERT || p->length == (TDMA_CAT_HEADER_SIZE - 1))
        return;

    ReliabilityTestPacket* pkt = (ReliabilityTestPacket*)p->payload;
    reliabilityResults[pkt->seq] |= 1 << (p->ttl + correction);
}

#include "TDMACAT.h"
void dump_tdma_table(int skip_blank)
{
    uBit.serial.printf("|SLOT\t|DID\t|EXP\t|DIST\t|FLGS\t|\r\n");
    uBit.serial.printf("|-------|-------|-------|-------|-------|\r\n");

    for (int i = 0; i < TDMA_CAT_TABLE_SIZE; i++)
    {
        TDMACATSlot t = tdma_get_slot(i);

        if (skip_blank && t.flags & TDMA_SLOT_FLAGS_UNINITIALISED)
            continue;

        uBit.serial.printf("|%d\t|%d\t|%d\t|%d\t|%d\t|\r\n", i, (int)t.device_identifier, t.expiration, t.distance, t.flags);
    }
    uBit.serial.printf("\r\n");
}


void clear_data(MicroBitEvent)
{
    memset(reliabilityResults, 0, TESTBED_MAX_SEQUENCE);
}

extern uint32_t serial_number;

#define PREALLOC_SLOT_SIZE 12
uint8_t slots[PREALLOC_SLOT_SIZE] = { 8, 12, 15, 20, 23, 27, 30, 35, 38, 40, 44, 48};

void pre_allocate_tdma()
{
    TDMACATSlot adv;

    adv.device_identifier = 0;
    adv.slot_identifier = TDMA_CAT_ADVERTISEMENT_SLOT;
    adv.expiration = TDMA_CAT_NEVER_EXPIRE;
    adv.distance = 0;
    adv.flags = 0;

    for (int i = 0 ; i < 12; i++)
    {
        adv.slot_identifier = slots[i];
        tdma_set_slot(adv, false);
    }
}

int main()
{
    uBit.init();
    // pre_allocate_tdma();
    uBit.radio.setTestRole(Observer);
    memset(reliabilityResults, 0, TESTBED_MAX_SEQUENCE);
    uBit.radio.enable();
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, transmit_results);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, clear_data);

    test_gpio_init();

    uBit.display.print((int)serial_number);
    uBit.sleep(500);

#if TEST_DISABLE_DISPLAY == 1
    uBit.display.disable();
#endif

    while(1)
    {
#if TEST_DISABLE_DISPLAY == 0
        uBit.display.print('O');
#endif
        uBit.sleep(500);
        // dump_tdma_table(1);
        // uBit.serial.printf("tx %d fwd %d rx %d crce: %d ", packets_transmitted, packets_forwarded, packets_received,crc_fail_count);
#if TEST_DISABLE_DISPLAY == 0
        uBit.display.print(' ');
#endif
        uBit.sleep(500);
    }
}

#endif