CC = gcc
CFLAGS = -O2 -std=c99 -pedantic -Wall -Wextra -Wno-unused-parameter

PREFIX = /usr/local

INC = -I/usr/include/libircclient
LIBS = -lircclient

OBJFILES = woofbot.o

all: woofbot

.PHONY: install uninstall clean

woofbot: $(OBJFILES)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

woofbot.o: config.h

%.o: %.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

install: woofbot
	install -m 0755 woofbot $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/woofbot

clean:
	rm -f *.o woofbot
