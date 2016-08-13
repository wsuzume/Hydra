CC = gcc
CFLAGS = -Wall -O2 -std=c11
LINKS = -lpthread
HEADER = type.h object.h

all: hydmaster.so hydkernel.so

hydmaster.so: master.h master.c object.c $(HEADER)
	$(CC) $(CFLAGS) -fPIC -c object.c -o object.lo
	$(CC) $(CFLAGS) -fPIC -c master.c -o hydmaster.lo
	$(CC) $(CFLAGS) -shared -Wl,-soname,hydmaster.so -o hydmaster.so hydmaster.lo object.lo

hydkernel.so: kernel.h kernel.c 
	$(CC) $(CFLAGS) -fPIC -c kernel.c -o hydkernel.lo
	$(CC) $(CFLAGS) -shared -Wl,-soname,hydkernel.so hydkernel.lo -o hydkernel.so
