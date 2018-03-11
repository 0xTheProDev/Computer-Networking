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
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [queueSize]\n", argv[0]);
        return -1;
    }
    const int queueSize = argc == 2 ? atoi(argv[1]) : MAX_CONN;
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
    printf("Server started at %s:%d\n", inet_ntoa(server.sin_addr), SOCKET_PORT);
    while (! signaled) {
        int client_addr_len = sizeof(client);
        int cfd = accept(sfd, (struct sockaddr *) &client, (socklen_t *) &client_addr_len);
        if (cfd < 0) {
            fprintf(stderr, "Failed to accept client connection\n");
            close(sfd);
            return -5;
        }
        printf("Established connection with client %s\n", inet_ntoa(client.sin_addr));
        char buffer[MSG_SIZE];
        memset(buffer, 0, sizeof(buffer));
        if (recv(cfd, buffer, sizeof(buffer), MSG_FLAG) == -1) {
            fprintf(stderr, "Failed to receive client message\n");
            close(cfd);
            close(sfd);
            return -6;
        }
        printf("Received message from client %s\n\t%s\n", inet_ntoa(client.sin_addr), buffer);
        if (send(cfd, buffer, sizeof(buffer), MSG_FLAG) == -1) {
            fprintf(stderr, "Failed to send back message to client\n");
            close(cfd);
            close(sfd);
            return -7;
        }
        printf("Sent back message to client %s\n\t%s\n", inet_ntoa(client.sin_addr), buffer);
        if (close(cfd)) {
            fprintf(stderr, "Failed to close connection with client %s\n", inet_ntoa(client.sin_addr));
            close(sfd);
            return -8;
        }
        printf("Closed connection with client %s\n", inet_ntoa(client.sin_addr));
    }
    if (close(sfd)) {
        fprintf(stderr, "Failed to close server socket\n");
        return -9;
    }
    return 0;
}
