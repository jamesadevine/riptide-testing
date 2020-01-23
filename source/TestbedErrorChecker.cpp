#include "TestConfig.h"
#include "GPIODebug.h"

#if CURRENT_TEST_CONFIG == TEST_CONFIG_TESTBED_ERROR_CHECKER

#pragma message "COMPILING ERROR CHECKER"

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

volatile int p1_counter = 0;
volatile int p2_counter = 0;

volatile int rssi_avg = 0;
PeridoFrameBuffer expected;

void process_packet(PeridoFrameBuffer* p, bool crc_ok, int rssi)
{
    if (!p || crc_ok)
        return;

    rssi = 127 - rssi;

    if (p->ttl)
    {
        p1_counter++;
        // rssi_avg += rssi;
    }
    else
    {
        p2_counter++;
        rssi_avg += rssi;
    }

    expected.ttl = p->ttl;
    uint8_t* buffPtr = (uint8_t*)p;
    uint8_t* expBuffPtr = (uint8_t*)&expected;

    uBit.serial.printf("-----------%d------------\r\n", p->ttl);

    for (int i = 0; i < MICROBIT_PERIDO_HEADER_SIZE + 4; i++)
        if (buffPtr[i] != expBuffPtr[i])
            uBit.serial.printf("idx: %d exp: %d act: %d\r\n", i, expBuffPtr[i], buffPtr[i]);
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

int normalise_rssi(int rssi)
{
    return rssi / 12;
}

void render_counters()
{
    int rssi_counter = p2_counter;

    // uBit.serial.printf("raw: %d count: %d avg: %d normalised: %d\r\n", rssi_avg, rssi_counter, rssi_avg / rssi_counter, normalise_rssi(rssi_avg / rssi_counter));

    if (p1_counter > 10)
        p1_counter = 10;

    if (p2_counter > 10)
        p2_counter = 10;

    bar_chart.clear();

    draw_chart(normalise_rssi(rssi_avg / rssi_counter), 0, 4, 2);
    draw_chart(p2_counter, 3, 4, 5);

    uBit.display.print(bar_chart);

    p1_counter = 0;
    p2_counter = 0;
    rssi_avg = 0;
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

    ReliabilityTestPacket* t = (ReliabilityTestPacket *)expected.payload;

    expected.length = 4 + MICROBIT_PERIDO_HEADER_SIZE - 1;
    expected.app_id = 0;
    expected.namespace_id = 0;
    expected.id = 0xff00;
    expected.ttl = MICROBIT_PERIDO_DEFAULT_TTL;
    expected.initial_ttl = MICROBIT_PERIDO_DEFAULT_TTL;
    expected.time_since_wake = 0;
    expected.period = 0;
    expected.flags = 0;
    t->seq = 999;

    uBit.messageBus.listen(MICROBIT_ID_BUTTON_A, MICROBIT_BUTTON_EVT_CLICK, onButtonA);
    uBit.messageBus.listen(MICROBIT_ID_BUTTON_B, MICROBIT_BUTTON_EVT_CLICK, onButtonB);
    uBit.radio.setTestRole(Observer);
    uBit.radio.enable();

    test_gpio_init();
    while(1)
    {
        render_counters();
        uBit.sleep(1000);
    }
}

// for (int i = 1; i < 11; i++)
// {
//     bar_chart.clear();
//     draw_chart(i, 0, 4, 2);
//     draw_chart(i, 3, 4, 5);
//     uBit.display.print(bar_chart);
//     uBit.sleep(1000);
// }

#endif