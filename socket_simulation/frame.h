#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>

#define MAX_PAYLOAD 256
#define MAX_RETRIES 5
#define TIMEOUT_MS 1000

typedef struct {
    uint8_t sender;
    uint8_t receiver;
    uint8_t length;
    char payload[MAX_PAYLOAD];
    uint8_t checksum;
} Frame;

uint8_t compute_checksum(Frame *frame);
void print_frame(Frame *frame, const char *tag);

#endif
