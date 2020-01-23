#include "MicroBit.h"

static int channel_number = MICROBIT_RADIO_DEFAULT_FREQUENCY;

void increment_channel_number(MicroBit& uBit)
{
    channel_number++;
    uBit.radio.setFrequencyBand(channel_number);
    uBit.display.print(channel_number);
    uBit.sleep(500);
}

void decrement_channel_number(MicroBit& uBit)
{
    channel_number--;
    uBit.radio.setFrequencyBand(channel_number);
    uBit.display.print(channel_number);
    uBit.sleep(500);
}