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

#define MAX_RETRIES 10

int main() {
    // Remove old files at start to avoid stale data problems
    remove(PATH_A_TO_B);
    remove(PATH_ACK_B_TO_A);

    int retries = 0;

    Frame frame;
    frame.sender = A_ADDR;
    frame.receiver = B_ADDR;
    const char *msg = "Hello from A!";
    frame.length = (uint8_t)strlen(msg);
    memcpy(frame.payload, msg, frame.length);
    frame.checksum = compute_checksum(&frame);

    while (retries < MAX_RETRIES) {
        FILE *out = fopen(PATH_A_TO_B, "wb");
        if (!out) {
            perror("A: Error opening file for writing");
            return 1;
        }
        fwrite(&frame, sizeof(Frame), 1, out);
        fclose(out);
        printf("[A -> B] Frame sent. Waiting for ACK... (try %d)\n", retries + 1);

        sleep(3);  // Wait longer to give B time to respond

        FILE *ack = fopen(PATH_ACK_B_TO_A, "rb");
        if (ack) {
            printf("A: ACK received from B\n");
            fclose(ack);
            break;
        } else {
            printf("A: No ACK yet. Retrying...\n");
            retries++;
        }
    }

    if (retries == MAX_RETRIES) {
        printf("A: Failed to receive ACK after %d retries\n", MAX_RETRIES);
    }

    return 0;
}
