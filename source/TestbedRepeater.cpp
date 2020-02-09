#include "TestConfig.h"
#include "GPIODebug.h"

#if CURRENT_TEST_CONFIG == TEST_CONFIG_TESTBED_REPEATER

#pragma message "COMPILING REPEATER"

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

void process_packet(TDMACATSuperFrame* p, bool crc_ok, int)
{
    if (!p || !crc_ok)
        return;

    ReliabilityTestPacket* pkt = (ReliabilityTestPacket*)p->payload;
    reliabilityResults[pkt->seq][p->ttl + 1] = 1;
}

int main()
{
    uBit.init();
    uBit.radio.setTestRole(Repeater);
    uBit.radio.enable();
    test_gpio_init();

    while(1)
    {
        uBit.display.print('R');
        uBit.sleep(500);
        uBit.display.print(' ');
        uBit.sleep(500);
    }
}

#endif