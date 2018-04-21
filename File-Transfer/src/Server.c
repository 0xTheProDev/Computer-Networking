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

/* Server Program for File Transfer */

#include <WebSocket.h>

int main(int argc, const char* argv[]) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [queueSize]\n", argv[0]);
        return -1;
    }
    const int queueSize = argc == 2 ? atoi(argv[1]) : 1024;
    struct sockaddr_in server, client;
    memset(&server, 0, sizeof(server));
    memset(&client, 0, sizeof(client));
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_family = AF_INET;
    server.sin_port = htons(SOCKET_PORT);
    int sfd = socket(AF_INET, SOCK_STREAM, SOCKET_PROTO);
    if (sfd == -1) {
        fprintf(stderr, "Error: Failed to create a server socket\n");
        return -2;
    }
    if (bind(sfd, (struct sockaddr*) &server, sizeof(server))) {
        fprintf(stderr, "Failed to bind server socket\n");
        close(sfd);
        return -3;
    }
    if (listen(sfd, queueSize)) {
        fprintf(stderr, "Failed to start server socket\n");
        close(sfd);
        return -4;
    }
    printf("Server started at %s:%d\n", inet_ntoa(server.sin_addr), 8080);
    int client_addr_len = sizeof(client);
    int cfd = accept(sfd, (struct sockaddr *) &client, (socklen_t *) &client_addr_len);
    if (cfd < 0) {
        fprintf(stderr, "Failed to accept client connection\n");
        close(sfd);
        return -5;
    }
    printf("Established connection with client %s\n", inet_ntoa(client.sin_addr));
    char sndBfr[MSG_SIZE], rcvBfr[MSG_SIZE];
    while (true) {
        memset(sndBfr, 0, sizeof(sndBfr));
        memset(rcvBfr, 0, sizeof(rcvBfr));
        if (recv(cfd, rcvBfr, sizeof(rcvBfr), MSG_FLAG) == -1) {
            fprintf(stderr, "Failed to receive message from %s\n", inet_ntoa(client.sin_addr));
            close(cfd);
            close(sfd);
            return -6;
        }
        if (strcasecmp(rcvBfr, "exit") == 0) break;
        printf("Received operation from %s: READ %s\n", inet_ntoa(client.sin_addr), rcvBfr);
        FILE* fp = fopen(rcvBfr, "r");
        if (fp == NULL) {
            sprintf(sndBfr, "Failed to open file %s", rcvBfr);
            fprintf(stderr, "%s\n", sndBfr);
            if (send(cfd, sndBfr, sizeof(sndBfr), MSG_FLAG) == -1) {
                fprintf(stderr, "Failed to send message to %s\n", inet_ntoa(client.sin_addr));
                close(cfd);
                close(sfd);
                return -7;
            }
            sprintf(sndBfr, "exit");
            if (send(cfd, sndBfr, sizeof(sndBfr), MSG_FLAG) == -1) {
                fprintf(stderr, "Failed to send message to %s\n", inet_ntoa(client.sin_addr));
                close(cfd);
                close(sfd);
                return -7;
            }
        } else {
            while (! feof(fp)) {
                fread(sndBfr, 1, MSG_SIZE, fp);
                if (send(cfd, sndBfr, sizeof(sndBfr), MSG_FLAG) == -1) {
                    fprintf(stderr, "Failed to send message to %s\n", inet_ntoa(client.sin_addr));
                    close(cfd);
                    close(sfd);
                    return -8;
                }
            }
            sprintf(sndBfr, "exit");
            if (send(cfd, sndBfr, sizeof(sndBfr), MSG_FLAG) == -1) {
                fprintf(stderr, "Failed to send message to %s\n", inet_ntoa(client.sin_addr));
                close(cfd);
                close(sfd);
                return -8;
            }
        }
        fclose(fp);
    }
    if (close(sfd)) {
        fprintf(stderr, "Failed to close server socket\n");
        return -9;
    }
    printf("Closed connection with client %s\n", inet_ntoa(client.sin_addr));
    return 0;
}
