#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep((x)*1000)
#else
#include <unistd.h>
#endif

#include "common.h"
#include "frame.h"

#define PATH_A_TO_B "A_to_B.bin"
#define PATH_ACK_B_TO_A "ACK_B_TO_A.bin"
#define PATH_B_TO_C "B_to_C.bin"

int main() {
    // Clean up stale files at startup
    remove(PATH_A_TO_B);
    remove(PATH_ACK_B_TO_A);
    remove(PATH_B_TO_C);

    printf("B: Waiting to receive frame from A...\n");
    sleep(5);  // Delay to give A time to send frame

    FILE *in = fopen(PATH_A_TO_B, "rb");
    if (!in) {
        printf("B: Frame file from A not found.\n");
        return 1;
    }

    Frame frame;
    size_t read = fread(&frame, sizeof(Frame), 1, in);
    fclose(in);

    if (read != 1) {
        printf("B: Failed to read full frame from A.\n");
        return 1;
    }

    if (compute_checksum(&frame) != frame.checksum) {
        printf("B: Corrupted frame from A.\n");
        return 1;
    }

    print_frame(&frame, "[B received]");

    // Send ACK to A
    Frame ack;
    ack.sender = B_ADDR;
    ack.receiver = A_ADDR;
    const char *ack_msg = "ACK from B";
    ack.length = (uint8_t)strlen(ack_msg);
    memcpy(ack.payload, ack_msg, ack.length);
    ack.checksum = compute_checksum(&ack);

    FILE *ack_file = fopen(PATH_ACK_B_TO_A, "wb");
    if (!ack_file) {
        perror("B: Failed to write ACK");
        return 1;
    }
    fwrite(&ack, sizeof(Frame), 1, ack_file);
    fclose(ack_file);
    printf("B: ACK sent to A.\n");

    // Forward frame to C
    Frame fwd = frame;
    fwd.sender = B_ADDR;
    fwd.receiver = C_ADDR;
    fwd.checksum = compute_checksum(&fwd);

    FILE *out = fopen(PATH_B_TO_C, "wb");
    if (!out) {
        perror("B: Failed to forward to C");
        return 1;
    }
    fwrite(&fwd, sizeof(Frame), 1, out);
    fclose(out);
    print_frame(&fwd, "[B -> C]");

    return 0;
}
