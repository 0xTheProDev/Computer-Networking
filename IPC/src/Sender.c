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

/* Sender program for IPC */

#include <Message.h>

int main(int argc, char* argv[]) {
	/* Check number of arguments */
	if (argc != 3) {
		fprintf(stderr, "Usage: %s [key-file] [msg-file]\n", argv[0]);
		return 1;
	}
	/* Create key from file given by user */
	key_t key = ftok(argv[1], RANDINT);
	if (key == -1) {
		fprintf(stderr, "Failed to create key\n");
		return 2;
	}
	/* Entry into message queue against key */
	int msgid = msgget(key, 0666 | IPC_CREAT);
	if (msgid == -1) {
		fprintf(stderr, "Failed to create message queue\n");
		return 3;
	}
	/* Open file handle for message file */
	FILE* fptr = fopen(argv[2], "r");
	if (fptr == NULL) {
		fprintf(stderr, "Failed to open file %s\n", argv[2]);
		return 4;
	}
	/* Create message variable */
	struct msgbuf msg;
	msg.mtype = MSGTYPE;
	/* Send message on the queue */
	while (fscanf(fptr, "%s", msg.mtext) > 0) {
		msgsnd(msgid, &msg, sizeof(msg), 0);
		printf("Sent message...\n%s\n\n", msg.mtext);
	}
	/* Close file handle */
	fclose(fptr);
	return 0;
}
