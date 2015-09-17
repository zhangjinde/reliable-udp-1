SHELL = /bin/sh
CC    = gcc
CFLAGS       = -fPIC -Wall -Werror -shared  

TARGET1  = librudp.so
SOURCES1 = rudp.c udp.c
HEADERS = rudp.h
OBJECTS1 = $(SOURCES1:.c=.o)

TARGET2  = server
SOURCES2 = server.c udp.c
OBJECTS2 = $(SOURCES2:.c=.o)

all: $(TARGET1) $(TARGET2) 

$(TARGET1): $(OBJECTS1)
	$(CC) $(CFLAGS)  -o $(TARGET1) $(OBJECTS1)

$(TARGET2): $(OBJECTS2)
	$(CC)   -o $(TARGET2) $(OBJECTS2)

clean:
	rm -rf $(TARGET1) $(TARGET2) $(OBJECTS1) $(OBJECTS2)
