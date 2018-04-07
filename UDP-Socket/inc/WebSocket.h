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

/* WebSocket.h: Header file written for experimenting with Socket Programming */

#ifndef __WebSocket_h
#define __WebSocket_h

#define _XOPEN_SOURCE

#ifndef __arpa_inet_h
    #include <arpa/inet.h>
#endif
#ifndef __signal_h
    #include <signal.h>
#endif
#ifndef __stdio_h
    #include <stdio.h>
#endif
#ifndef __stdbool_h
    #include <stdbool.h>
#endif
#ifndef __stdlib_h
    #include <stdlib.h>
#endif
#ifndef __string_h
    #include <string.h>
#endif
#ifndef __sys_socket_h
    #include <sys/socket.h>
#endif
#ifndef __sys_types_h
    #include <sys/types.h>
#endif
#ifndef __unistd_h
    #include <unistd.h>
#endif

#ifndef SOCKET_IP
    #define SOCKET_IP "127.0.0.1"
#endif
#ifndef SOCKET_PORT
    #define SOCKET_PORT 8080
#endif
#ifndef SOCKET_PROTO
    #define SOCKET_PROTO 0
#endif
#ifndef MSG_SIZE
    #define MSG_SIZE 128
#endif
#ifndef MSG_FLAG
    #define MSG_FLAG 0
#endif
#ifndef MAX_CONN
    #define MAX_CONN 10
#endif

#endif
