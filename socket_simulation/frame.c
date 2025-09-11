#include "frame.h"
#include <stdio.h>

uint8_t compute_checksum(Frame *frame) {
    uint8_t cs = 0;
    cs ^= frame->sender;
    cs ^= frame->receiver;
    cs ^= frame->length;
    for(int i = 0; i < frame->length; i++) {
        cs ^= frame->payload[i];
    }
    return cs;
}

void print_frame(Frame *frame, const char *tag) {
    printf("[%s] Frame from 0x%02X to 0x%02X: \"%.*s\" (checksum: 0x%02X)\n",
           tag, frame->sender, frame->receiver, frame->length, frame->payload, frame->checksum);
}
