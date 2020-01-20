#include "TestConfig.h"
#include "GPIODebug.h"

#if CURRENT_TEST_CONFIG == TEST_CONFIG_TESTBED_VISUALISER

#pragma message "COMPILING VISUALISER"

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

void process_packet(PeridoFrameBuffer* p, bool crc_ok)
{
    if (!p || !crc_ok)
        return;

    if (p->ttl)
        p1_counter++;
    else
        p2_counter++;
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
    if (p1_counter > 10)
        p1_counter = 10;

    if (p2_counter > 10)
        p2_counter = 10;

    bar_chart.clear();

    draw_chart(p1_counter, 0, 4, 2);
    draw_chart(p2_counter, 3, 4, 5);

    uBit.display.print(bar_chart);

    p1_counter = 0;
    p2_counter = 0;
}

int main()
{
    uBit.init();
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