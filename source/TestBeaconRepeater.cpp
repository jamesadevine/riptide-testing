#include "TestConfig.h"
#include "GPIODebug.h"

#if CURRENT_TEST_CONFIG == TEST_CONFIG_TESTBED_BEACON_REPEATER

#pragma message "COMPILING BEACON REPEATER"

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

MicroBitImage bar_chart(5,5);

void draw_chart(int counter, int startx, int starty, int endx)
{
    int y = starty;
    int x = startx;

    while(counter > 0)
    {
        bar_chart.setPixelValue(x, y, 1);

        counter--;

        x++;

        if (x % endx == 0)
        {
            y--;
            x -= 2;
        }
    }
}

void render_counters()
{
    if (packets_received > 10)
        packets_received = 10;

    if (packets_forwarded > 10)
        packets_forwarded = 10;

    bar_chart.clear();

    draw_chart(packets_received, 0, 4, 2);
    draw_chart(packets_forwarded, 3, 4, 5);

    uBit.display.print(bar_chart);

    packets_received = 0;
    packets_forwarded = 0;
}

extern void increment_channel_number(MicroBit&);
void onButtonA(MicroBitEvent)
{
    increment_channel_number(uBit);
}

extern void decrement_channel_number(MicroBit&);
void onButtonB(MicroBitEvent)
{
    decrement_channel_number(uBit);
}

int main()
{
    uBit.init();
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    uBit.radio.setTestRole(Repeater);
    uBit.radio.enable();
    test_gpio_init();

    uBit.display.print('R');

    while(1)
    {
        uBit.sleep(1000);
        render_counters();
    }
}

#endif