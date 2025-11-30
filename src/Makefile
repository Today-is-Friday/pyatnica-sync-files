CC=gcc
cflags=-Wall -Werror -Wextra -std=c11
dflags=-g -O0

.PHONY: all debug 

all: build

build: segodnyaconnect.h segodnyaconnect.c config_file.c 
	$(CC) $(cflags) segodnyaconnect.c config_file.c -o segodnyaconnect

debug: segodnyaconnect.h segodnyaconnect.c config_file.c 
	$(CC) $(cflags) $(dflags) segodnyaconnect.c config_file.c -o segodnyaconnect

daemon: segodnyaconnectd.c file_handle.h config_file.c
	$(CC) $(cflags) $(dflags) segodnyaconnectd.c config_file.c -o segodnyaconnectd

valgrind-daemon:
	valgrind --tool=memcheck --leak-check=yes ./segodnyaconnectd 

remake: clean build

clean:
	rm -rf segodnyaconnect* 