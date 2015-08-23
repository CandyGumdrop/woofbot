CC = gcc
CFLAGS = -O2 -std=c99 -pedantic -Wall -Wextra -Wno-unused-parameter

PREFIX = /usr/local

INC = /usr/include/libircclient
LIBS = -lircclient

SRCDIR = src
OBJDIR = $(TGT)/obj

OBJFILES = woofbot.o

all: woofbot

.PHONY: install uninstall clean

woofbot: $(OBJFILES)
	$(CC) $(CFLAGS) -I$(INC) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

install: woofbot
	install -m 0755 woofbot $(PREFIX)/bin

uninstall:
	rm -f $(PREFIX)/bin/woofbot

clean:
	rm -f *.o woofbot
