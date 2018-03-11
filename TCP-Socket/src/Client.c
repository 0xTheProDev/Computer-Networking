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

#include <WebSocket.h>

bool signaled = false;

void handler (int signal) {
    if (signal == SIGINT) {
        signaled = true;
    }
}

int main(int argc, const char* argv[]) {
    signal(SIGINT, handler);
    if (argc > 3) {
        fprintf(stderr, "Usage: %s [Server_IP] [Server_Port]\n", argv[0]);
        return -1;
    }
    const char* host = argc > 1 ? argv[1] : SOCKET_IP;
    const int port = argc == 3 ? atoi(argv[2]) : SOCKET_PORT;
    struct sockaddr_in server;
    memset(&server, 0, sizeof(server));
    server.sin_addr.s_addr = inet_addr(host);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    int sfd = socket(AF_INET, SOCK_STREAM, SOCKET_PROTO);
    if (sfd == -1) {
        fprintf(stderr, "Error: Failed to create a server socket\n");
        return -2;
    }
    if (connect(sfd, (struct sockaddr*) &server, sizeof(server))) {
        fprintf(stderr, "Failed to connect with server socket\n");
        close(sfd);
        return -3;
    }
    char buffer[MSG_SIZE], echo[MSG_SIZE];
    while (! signaled) {
        memset(buffer, 0, sizeof(buffer));
        printf("Enter the message you want to send: [Press Ctrl+C to stop]\n");
        scanf("%s", buffer);
        if (! strlen(buffer)) continue;
        printf("Sending message to %s\n\t%s\n", inet_ntoa(server.sin_addr), buffer);
        if (send(sfd, buffer, sizeof(buffer), MSG_FLAG) == -1) {
            fprintf(stderr, "Failed to send message to server\n");
            close(sfd);
            return -4;
        }
        memset(echo, 0, sizeof(echo));
        if (recv(sfd, echo, sizeof(echo), MSG_FLAG) == -1) {
            fprintf(stderr, "Failed to receive back from server\n");
            close(sfd);
            return -5;
        }
        printf("Received message from %s\n\t%s\n", inet_ntoa(server.sin_addr), buffer);
    }
    if (close(sfd)) {
        fprintf(stderr, "Failed to close server socket\n");
        return -6;
    }
    return 0;
}
