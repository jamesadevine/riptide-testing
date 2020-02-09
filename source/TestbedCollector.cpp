#include "TestConfig.h"
#include "GPIODebug.h"

#if CURRENT_TEST_CONFIG == TEST_CONFIG_TESTBED_COLLECTOR

#pragma message "COMPILING COLLECTOR"

#include "MicroBit.h"

MicroBit uBit;

struct ReliabilityTestPacket
{
    uint32_t seq;
}__attribute__((packed));

extern volatile uint32_t packets_received;
extern volatile uint32_t packets_transmitted;
extern volatile uint32_t packets_forwarded;
extern volatile uint32_t crc_fail_count;

void process_packet(TDMACATSuperFrame*, bool, int)
{
}

void print_packet(TDMACATTestFrame* p, bool crc_ok, int)
{
    if (!p)
        return;

    if (!crc_ok)
    {
        uBit.serial.printf("CRC ERROR");
        return;
    }

    uint8_t iterator = 0;

    uint16_t* seq_start = (uint16_t *)p->payload;
    uint16_t seq_counter = *seq_start;
    iterator = TESTBED_RESULT_HEADER_SIZE;

    while (iterator < TESTBED_MAX_PAYLOAD + TESTBED_RESULT_HEADER_SIZE && seq_counter < TESTBED_MAX_SEQUENCE)
    {
        uint8_t byte = p->payload[iterator++];
        uBit.serial.printf("%d, ", seq_counter++);
        for (int i = 0; i < 8; i++)
            if (i == 7)
                uBit.serial.printf("%d\r\n", ((byte & (1 << i)) > 0) ? 1 : 0);
            else
                uBit.serial.printf("%d, ", ((byte & (1 << i)) > 0) ? 1 : 0);
    }
}

TDMACATTestFrame staticBuff;

int main()
{
    uBit.init();
    test_gpio_init();

    NRF_CLOCK->EVENTS_HFCLKSTARTED = 0;
    NRF_CLOCK->TASKS_HFCLKSTART = 1;
    while (NRF_CLOCK->EVENTS_HFCLKSTARTED == 0);

    NRF_RADIO->POWER = 0;
    NRF_RADIO->POWER = 1;

    // NRF_RADIO->TXPOWER = (uint32_t)MICROBIT_BLE_POWER_LEVEL[0];
    NRF_RADIO->TXPOWER = (uint32_t)MICROBIT_BLE_POWER_LEVEL[6];
    NRF_RADIO->FREQUENCY = MICROBIT_RADIO_DEFAULT_FREQUENCY;

    // Configure for 1Mbps throughput.
    // This may sound excessive, but running a high data rates reduces the chances of collisions...
    NRF_RADIO->MODE = RADIO_MODE_MODE_Nrf_1Mbit;
    NRF_RADIO->BASE0 =  MICROBIT_RADIO_BASE_ADDRESS;
    NRF_RADIO->PREFIX0 = 0;

    // The RADIO hardware module supports the use of multiple addresses, but as we're running anonymously, we only need one.
    // Configure the RADIO module to use the default address (address 0) for both send and receive operations.
    NRF_RADIO->TXADDRESS = 0;
    NRF_RADIO->RXADDRESSES = 1;

    // Packet layout configuration. The nrf51822 has a highly capable and flexible RADIO module that, in addition to transmission
    // and reception of data, also contains a LENGTH field, two optional additional 1 byte fields (S0 and S1) and a CRC calculation.
    // Configure the packet format for a simple 8 bit length field and no additional fields.
    NRF_RADIO->PCNF0 = 0x00000008;
    // NRF_RADIO->PCNF1 = 0x02040000 | MICROBIT_RADIO_MAX_PACKET_SIZE;
    // NRF_RADIO->PCNF1 = 0x00040000 | MICROBIT_RADIO_MAX_PACKET_SIZE;

    NRF_RADIO->PCNF1 = 0x02040000 | 252;

    // Most communication channels contain some form of checksum - a mathematical calculation taken based on all the data
    // in a packet, that is also sent as part of the packet. When received, this calculation can be repeated, and the results
    // from the sender and receiver compared. If they are different, then some corruption of the data ahas happened in transit,
    // and we know we can't trust it. The nrf51822 RADIO uses a CRC for this - a very effective checksum calculation.
    //
    // Enable automatic 16bit CRC generation and checking, and configure how the CRC is calculated.
    NRF_RADIO->CRCCNF = RADIO_CRCCNF_LEN_Two;
    NRF_RADIO->CRCINIT = 0xFFFF;
    NRF_RADIO->CRCPOLY = 0x11021;

    // Set the start random value of the data whitening algorithm. This can be any non zero number.
    NRF_RADIO->DATAWHITEIV = MICROBIT_RADIO_DEFAULT_FREQUENCY;

    NRF_RADIO->EVENTS_READY = 0;
    NRF_RADIO->EVENTS_END = 0;
    // NRF_RADIO->TIFS = 300;
    NRF_RADIO->PACKETPTR = (uint32_t)&TDMACATRadio::instance->staticFrame;
    NRF_RADIO->SHORTS = RADIO_SHORTS_READY_START_Msk | RADIO_SHORTS_END_DISABLE_Msk | RADIO_SHORTS_ADDRESS_RSSISTART_Msk;

    NRF_RADIO->INTENCLR = 0xffffffff;
    NRF_RADIO->PACKETPTR = (uint32_t)&staticBuff;

    while(1)
    {
        uBit.display.print('C');
        NRF_RADIO->EVENTS_READY = 0;
        NRF_RADIO->TASKS_RXEN = 1;
        while(NRF_RADIO->EVENTS_READY == 0);
        NRF_RADIO->EVENTS_END = 0;
        NRF_RADIO->TASKS_START =1;
        while(NRF_RADIO->EVENTS_END == 0);

        print_packet(&staticBuff, NRF_RADIO->CRCSTATUS == 1, 0);
    }
}

#endif