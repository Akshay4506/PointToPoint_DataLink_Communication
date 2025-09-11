#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "frame.h"

#pragma comment(lib, "ws2_32.lib")

#define A_ADDR 0x01
#define B_ADDR 0x02
#define C_ADDR 0x03
#define PORT_B 5001
#define PORT_C 5002

int main() {
    WSADATA wsa;
    SOCKET sock_b, sock_c;
    struct sockaddr_in addr_b, addr_a, addr_c;
    int addr_len = sizeof(addr_a);
    Frame frame;
    int drop_rate = 30; // Drop 30% of frames randomly to simulate error

    WSAStartup(MAKEWORD(2,2), &wsa);

    // Socket for receiving from A
    sock_b = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    addr_b.sin_family = AF_INET;
    addr_b.sin_addr.s_addr = INADDR_ANY;
    addr_b.sin_port = htons(PORT_B);
    bind(sock_b, (struct sockaddr*)&addr_b, sizeof(addr_b));

    // Socket for sending to C
    sock_c = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    addr_c.sin_family = AF_INET;
    addr_c.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_c.sin_port = htons(PORT_C);

    srand((unsigned int)time(NULL));

    while(1) {
        int ret = recvfrom(sock_b, (char*)&frame, sizeof(Frame), 0,
                           (struct sockaddr*)&addr_a, &addr_len);
        if(ret == SOCKET_ERROR) {
            printf("B: recvfrom failed\n");
            break;
        }
        print_frame(&frame, "B received");

        // Simulate random drop
        int drop = (rand() % 100) < drop_rate;
        if(drop) {
            printf("B: Simulated frame drop. Not sending ACK or forwarding.\n");
            continue;
        }

        // Send ACK back to A
        Frame ack = {0};
        ack.sender = B_ADDR;
        ack.receiver = A_ADDR;
        ack.length = 0;
        ack.checksum = compute_checksum(&ack);
        sendto(sock_b, (const char*)&ack, sizeof(Frame), 0,
               (struct sockaddr*)&addr_a, addr_len);
        printf("B: ACK sent to A.\n");

        // Forward frame to C
        frame.sender = B_ADDR;
        frame.receiver = C_ADDR;
        frame.checksum = compute_checksum(&frame);
        sendto(sock_c, (const char*)&frame, sizeof(Frame), 0,
               (struct sockaddr*)&addr_c, sizeof(addr_c));
        printf("B: Forwarded frame to C.\n");
    }

    closesocket(sock_b);
    closesocket(sock_c);
    WSACleanup();
    return 0;
}
