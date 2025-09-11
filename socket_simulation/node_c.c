#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include "frame.h"

#pragma comment(lib, "ws2_32.lib")

#define B_ADDR 0x02
#define C_ADDR 0x03
#define PORT_C 5002

int main() {
    WSADATA wsa;
    SOCKET sock_c;
    struct sockaddr_in addr_c, addr_b;
    int addr_len = sizeof(addr_b);
    Frame frame;

    WSAStartup(MAKEWORD(2,2), &wsa);

    sock_c = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    addr_c.sin_family = AF_INET;
    addr_c.sin_addr.s_addr = INADDR_ANY;
    addr_c.sin_port = htons(PORT_C);
    bind(sock_c, (struct sockaddr*)&addr_c, sizeof(addr_c));

    printf("Node C: Waiting for frames...\n");

    while(1) {
        int ret = recvfrom(sock_c, (char*)&frame, sizeof(Frame), 0,
                           (struct sockaddr*)&addr_b, &addr_len);
        if(ret == SOCKET_ERROR) {
            printf("C: recvfrom failed\n");
            break;
        }

        // Check checksum
        uint8_t cs = compute_checksum(&frame);
        if(cs != frame.checksum) {
            printf("C: Invalid checksum! Frame discarded.\n");
            continue;
        }
        print_frame(&frame, "C received");
    }

    closesocket(sock_c);
    WSACleanup();
    return 0;
}
