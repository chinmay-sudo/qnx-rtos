DEBUG = -g
CC = qcc
LD = qcc

TARGET = -Vgcc_ntox86_64
#TARGET = -Vgcc_ntoaarch64le
CFLAGS += $(DEBUG) $(TARGET) -Wall
LDFLAGS += $(DEBUG) $(TARGET)

BINS = Unit-3-IPC/01-message-passing/msg_client Unit-3-IPC/01-message-passing/msg_server

all: ${BINS}


clean:
	rm -f Unit-3-IPC/01-message-passing/msg_client Unit-3-IPC/01-message-passing/msg_server
