#include "TestConfig.h"
#include "GPIODebug.h"

#if CURRENT_TEST_CONFIG == TEST_CONFIG_TESTBED_BEACON

#pragma message "COMPILING BEACON"

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

void process_packet(PeridoFrameBuffer* p, bool crc_ok, int)
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

extern void increment_channel_number(MicroBit&);
extern void decrement_channel_number(MicroBit&);

void select_frequency()
{
    uBit.display.print("?");

    int interaction_count = 0;

    while(1)
    {
        interaction_count++;

        if (uBit.buttonA.isPressed())
        {
            interaction_count = 0;
            increment_channel_number(uBit);
        }

        if (uBit.buttonB.isPressed())
        {
            interaction_count = 0;
            decrement_channel_number(uBit);
        }

        uBit.sleep(250);

        if (interaction_count >= 24)
            break;
    }
}

extern void manual_poke(PeridoFrameBuffer*);

PeridoFrameBuffer buf;

int main()
{
    uBit.init();
    uBit.radio.setTestRole(Transmitter);
    uBit.radio.enable();
    test_gpio_init();

    ReliabilityTestPacket* t = (ReliabilityTestPacket *)buf.payload;

    buf.length = 4 + MICROBIT_PERIDO_HEADER_SIZE - 1;
    buf.app_id = 0;
    buf.namespace_id = 0;
    buf.id = 0xff00;
    buf.ttl = MICROBIT_PERIDO_DEFAULT_TTL;
    buf.initial_ttl = MICROBIT_PERIDO_DEFAULT_TTL;
    buf.time_since_wake = 0;
    buf.period = 0;
    buf.flags = 0;
    t->seq = 999;

    uBit.display.print('B');
    uBit.sleep(1000);
    select_frequency();

    int packet_counter = 0;

    while(1)
    {
        wait_ms((1000/PACKETS_PER_SECOND)/2);
        manual_poke(&buf);
        wait_ms((1000/PACKETS_PER_SECOND)/2);

        packet_counter++;

        if (packet_counter % PACKETS_PER_SECOND == 0)
            render_counters();
    }
}

#endif