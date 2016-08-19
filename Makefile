CC = gcc
CFLAGS = -Wall -O2 -std=c11
LINKS = -lpthread -ldl

all: hydra

hydra: main.c
	$(CC) $(CFLAGS) -o hydra main.c -ldl

test: test.c
	$(CC) $(CFLAGS) -o test test.c -ldl
