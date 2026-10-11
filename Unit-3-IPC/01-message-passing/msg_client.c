/*
 * msg_client.c
 *
 * Client code for QNX Message Passing checksum example
 * Sends a string to the server and receives its checksum back
 */

#include <stdio.h>			// printf(), perror()
#include <stdlib.h>			// atoi(), exit(), EXIT_SUCCESS, EXIT_FAILURE
#include <string.h>			// String handling functions
#include <sys/neutrino.h>	// QNX neutrino IPC APIs
#include "msg_def.h"		// Shared message structure and constants

int main(int argc, char *argv[]){
	cksum_msg_t msg;
	int incoming_checksum;
	int status;
	int coid;
	int server_pid;
	int server_chid;

	if(argc != 4){
		printf("ERROR: Must be started with command line arguments\n");
		printf("Usage: %s <server_pid> <server_chid> <string>\n",argv[0]);
		return EXIT_FAILURE;
	}

	// store the read arguments
	server_pid = atoi(argv[1]);
	server_chid = atoi(argv[2]);

	// Connect to the server
	printf("Attempting to establish connection with server pid = %d and chid = %d \n", server_pid, server_chid);

	coid = ConnectAttach(0,server_pid,server_chid,_NTO_SIDE_CHANNEL,0);

	// ERROR handling for ConnectAttach()
	if(coid == -1){
		perror("ConnectAttach()");
		exit(EXIT_FAILURE);
	}

	msg.msg_type = CKSUM_MSG_TYPE;

	// Copy the message whose checksum is to be calculated from the argument safely to msg.string_to_cksum
	strlcpy(msg.string_to_cksum, argv[3], sizeof(msg.string_to_cksum));

	printf("Sending string: %s \n", msg.string_to_cksum);

	// Send string to server using MsgSend()
	status = MsgSend(coid,&msg, sizeof(msg), &incoming_checksum, sizeof(incoming_checksum));

	// ERROR handling for MsgSend()
	if(status == -1){
		perror("MsgSend()");
		exit(EXIT_FAILURE);
	}

	printf("Received checksum = %d from server \n", incoming_checksum);

	printf("MsgSend return status: %d \n",status);

	return EXIT_SUCCESS;


}

