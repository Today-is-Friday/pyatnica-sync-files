CC=gcc
cflags=-Wall -Werror -Wextra -std=c17
dflags=-g -O0
tflags=-lcheck -lsubunit -lm

.PHONY: all debug 

all: build

build: segodnyaconnect.h segodnyaconnect.c config_file.c 
	$(CC) $(cflags) segodnyaconnect.c config_file.c -o segodnyaconnect

debug: segodnyaconnect.h segodnyaconnect.c config_file.c 
	$(CC) $(cflags) $(dflags) segodnyaconnect.c config_file.c -o segodnyaconnect

daemon: segodnyaconnectd.c file_handle.h config_file.c
	$(CC) $(cflags) segodnyaconnectd.c config_file.c -o segodnyaconnectd

test: tests/test.h tests/test.c tests/test_config_file.c config_file.c file_handle.h
	$(CC) $(cflags) $(tflags) tests/test.c tests/test_config_file.c config_file.c -o test_config
	./test_config

valgrind-daemon:
	valgrind --tool=memcheck --leak-check=yes ./segodnyaconnectd 

remake: clean build

clean:
	rm -rf segodnyaconnect* 