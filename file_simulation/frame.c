#include <stdio.h>
#include "frame.h"
#include <string.h>

uint8_t compute_checksum(Frame *frame) {
    uint8_t sum = 0;
    sum += frame->sender;
    sum += frame->receiver;
    sum += frame->length;
    for (int i = 0; i < frame->length; i++) {
        sum += frame->payload[i];
    }
    return sum;
}

void print_frame(Frame *frame, const char *label) {
    printf("[%s] Frame from 0x%02X to 0x%02X: %.*s (checksum: 0x%02X)\n",
           label,
           frame->sender,
           frame->receiver,
           frame->length,
           frame->payload,
           frame->checksum);
}
