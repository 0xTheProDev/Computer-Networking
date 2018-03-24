/**
 * Copyright (c) 2018 Progyan Bhattacharya <progyanb@acm.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/* Server for Chat Program */

#include <WebSocket.h>

int main(int argc, const char* argv[]) {
    struct sockaddr_in server;
    socklen_t client_addr_len = sizeof(server);
    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = AF_INET;
    server.sin_port = htons(SOCKET_PORT);
    int sfd = socket(AF_INET, SOCK_DGRAM, SOCKET_PROTO);
    if (sfd == -1) {
        fprintf(stderr, "Error: Failed to create a server socket\n");
        return -2;
    }
    if (bind(sfd, (struct sockaddr *) &server, sizeof(server))) {
        fprintf(stderr, "Failed to bind server socket\n");
        return -3;
    }
    char sndBfr[MSG_SIZE], rcvBfr[MSG_SIZE];
    while (true) {
        memset(sndBfr, 0, sizeof(sndBfr));
        memset(rcvBfr, 0, sizeof(rcvBfr));
        if (recvfrom(sfd, rcvBfr, sizeof(rcvBfr), MSG_FLAG, (struct sockaddr *) &server, &client_addr_len) == -1) {
            fprintf(stderr, "Failed to receive client message\n");
            return -6;
        }
        printf("Received message from client %s\n\t%s\n", inet_ntoa(server.sin_addr), rcvBfr);
        if (strcasecmp(rcvBfr, "exit") == 0) break;
        printf("Enter the message you want to send:\n");
        scanf("%s", sndBfr);
        if (sendto(sfd, sndBfr, sizeof(sndBfr), MSG_FLAG, (struct sockaddr *) &server, sizeof(server)) == -1) {
            fprintf(stderr, "Failed to send back message to client\n");
            return -7;
        }
        printf("Sent back message to client %s\n\t%s\n", inet_ntoa(server.sin_addr), sndBfr);
    }
    if (close(sfd)) {
        fprintf(stderr, "Failed to close server socket\n");
        return -8;
    }
    return 0;
}
