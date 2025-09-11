#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>

#define PAYLOAD_SIZE 256
#define A_ADDR 0x01
#define B_ADDR 0x02

typedef struct {
    uint8_t sender;
    uint8_t receiver;
    uint8_t payload[PAYLOAD_SIZE];
    uint8_t length;
    uint8_t checksum;
} Frame;

uint8_t compute_checksum(Frame *frame);
void print_frame(Frame *frame, const char *label);

#endif
