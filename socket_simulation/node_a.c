#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include "frame.h"

#pragma comment(lib, "ws2_32.lib")

#define A_ADDR 0x01
#define B_ADDR 0x02
#define PORT_A 5000
#define PORT_B 5001

int main() {
    WSADATA wsa;
    SOCKET sock;
    struct sockaddr_in addr_b;
    int addr_len = sizeof(addr_b);
    Frame frame, ack;
    int retries = 0;

    WSAStartup(MAKEWORD(2,2), &wsa);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Bind A to its port
    struct sockaddr_in addr_a = {0};
    addr_a.sin_family = AF_INET;
    addr_a.sin_addr.s_addr = INADDR_ANY;
    addr_a.sin_port = htons(PORT_A);
    bind(sock, (struct sockaddr*)&addr_a, sizeof(addr_a));

    // Set B's address
    addr_b.sin_family = AF_INET;
    addr_b.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_b.sin_port = htons(PORT_B);

    // Prepare frame
    frame.sender = A_ADDR;
    frame.receiver = B_ADDR;
    const char *msg = "Hello from A!";
    frame.length = (uint8_t)strlen(msg);
    memcpy(frame.payload, msg, frame.length);
    frame.checksum = compute_checksum(&frame);

    while(retries < MAX_RETRIES) {
        sendto(sock, (const char*)&frame, sizeof(Frame), 0,
               (struct sockaddr*)&addr_b, sizeof(addr_b));
        printf("[A -> B] Sent frame. Waiting for ACK...\n");

        // Set socket timeout for recvfrom
        DWORD timeout = TIMEOUT_MS;
        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));

        int ret = recvfrom(sock, (char*)&ack, sizeof(Frame), 0,
                           (struct sockaddr*)&addr_b, &addr_len);

        if(ret > 0 && ack.receiver == A_ADDR && ack.sender == B_ADDR && ack.length == 0) {
            printf("A: ACK received from B\n");
            break;
        } else {
            printf("A: No ACK. Retrying...\n");
            retries++;
        }
    }

    if(retries == MAX_RETRIES) {
        printf("A: Failed to receive ACK after %d retries\n", MAX_RETRIES);
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}
