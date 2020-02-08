#ifndef TEST_CONFIG_H
#define TEST_CONFIG_H

#define TEST_CONFIG_TESTBED_OBSERVER        5
#define TEST_CONFIG_TESTBED_REPEATER        6
#define TEST_CONFIG_TESTBED_TRANSMITTER     7
#define TEST_CONFIG_TESTBED_COLLECTOR       8
#define TEST_CONFIG_TESTBED_VISUALISER      9
#define TEST_CONFIG_TESTBED_BEACON          10

#define CURRENT_TEST_CONFIG         TEST_CONFIG_TESTBED_REPEATER

#define TEST_SOURCE_COUNT                   1
#define TEST_PACKET_COUNT                   1000

#define TEST_SOURCE_ID                      0

#define WAIT_BETWEEN_PACKETS                (10)

#define TESTBED_MAX_SEQUENCE                1000
#define TESTBED_REPETITIONS_SEQUENCE        2
#define TESTBED_MAX_PAYLOAD                 80

#define PACKETS_PER_SECOND                  10

#endif