CC = gcc
CFLAGS = -O2 -std=c99 -pedantic -Wall -Wextra

INC = /usr/include/libircclient
LIBS = -lircclient

SRCDIR = src
OBJDIR = $(TGT)/obj

OBJFILES = woofbot.o

all: woofbot

woofbot: $(OBJFILES)
	$(CC) $(CFLAGS) -I$(INC) $^ -o $@ $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -I$(INC) -c $< -o $@

clean:
	rm -f *.o woofbot
