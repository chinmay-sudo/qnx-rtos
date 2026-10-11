/*
 * msg_server.c
 * Server code for QNX Message Passing checksum example
 * Receives a string from client and replies it back with its calculated checksum
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/neutrino.h>
#include "msg_def.h"

int calculate_checksum(char *text);

int main(void){

	int chid;
	int pid;
	rcvid_t rcvid;
	cksum_msg_t msg;
	int status;
	int checksum;

	// Create Channel
	chid = ChannelCreate(0);

	// ERROR handling for ChannelCreate()
	if(chid == -1){
		perror("ChannelCreate()");
		exit(EXIT_FAILURE);
	}

	pid = getpid(); // Get the server process's pid

	printf("Server started with pid: %d, chid: %d\n", pid, chid);

	while(1){

		// Check for messages from clients
		rcvid = MsgReceive(chid, &msg, sizeof(msg), NULL);

		// ERROR handling for MsgReceive()
		if (rcvid == -1) {
			perror("MsgReceive()");
			exit(EXIT_FAILURE);
		}

		// Check for msg type received
		if (msg.msg_type == CKSUM_MSG_TYPE) {

			printf("Got a checksum message\n");

			// Calculate the checksum of received string
			checksum = calculate_checksum(msg.string_to_cksum);

			// Reply back to the received string with its calculated checksum
			status = MsgReply(rcvid, EOK, &checksum, sizeof(checksum));

			// ERROR handling for MsgReply()
			if (status == -1) {
				perror("MsgReply()");
			}
		}
		else{
			printf("Got an unknown message type %d \n",msg.msg_type);

			// Reply error to the client
			if(MsgError(rcvid, ENOSYS) == -1){
				perror("MsgError()");
			}
		}

	}

	return EXIT_SUCCESS;
}


int calculate_checksum(char *text)
{
    char *c;
    int cksum = 0;

    for (c = text; *c != '\0'; c++) {
        cksum += *c;
    }

    return cksum;
}
