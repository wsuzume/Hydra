CC = gcc
CFLAGS = -Wall -O2 -std=c11
LINKS = -lpthread
HEADER = type.h object.h

all: hydmaster.so hydkernel.so

hydmaster.so: master.h master.c object.c type.c typesys.c lex.c lex.h $(HEADER)
	$(CC) $(CFLAGS) -fPIC -c type.c -o type.lo
	$(CC) $(CFLAGS) -fPIC -c typesys.c -o typesys.lo
	$(CC) $(CFLAGS) -fPIC -c object.c -o object.lo
	$(CC) $(CFLAGS) -fPIC -c lex.c -o lex.lo
	$(CC) $(CFLAGS) -fPIC -c master.c -o hydmaster.lo
	$(CC) $(CFLAGS) -shared -Wl,-soname,hydmaster.so -o hydmaster.so hydmaster.lo type.lo typesys.lo object.lo lex.lo

hydkernel.so: kernel.h kernel.c 
	$(CC) $(CFLAGS) -fPIC -c kernel.c -o hydkernel.lo
	$(CC) $(CFLAGS) -shared -Wl,-soname,hydkernel.so hydkernel.lo -o hydkernel.so

.PHONY: clean
clean:
	-rm *.lo
