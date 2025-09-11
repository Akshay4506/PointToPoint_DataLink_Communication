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

#define PATH_B_TO_C "B_to_C.bin"

int main() {
    // Clean up old frame
    remove(PATH_B_TO_C);

    printf("C: Waiting to receive frame from B...\n");
    sleep(5);  // Give B time to send the frame

    FILE *in = fopen(PATH_B_TO_C, "rb");
    if (!in) {
        printf("C: Frame file from B not found.\n");
        return 1;
    }

    Frame frame;
    size_t read = fread(&frame, sizeof(Frame), 1, in);
    fclose(in);

    if (read != 1) {
        printf("C: Failed to read full frame.\n");
        return 1;
    }

    if (compute_checksum(&frame) != frame.checksum) {
        printf("C: Corrupted frame received.\n");
        return 1;
    }

    print_frame(&frame, "[C received]");

    return 0;
}
