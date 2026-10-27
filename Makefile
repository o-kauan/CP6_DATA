CC=gcc
CFLAGS=-Wall -Wextra -std=c11 -O2
BIN=bin/hanoi_engine
all:
	mkdir -p bin
	$(CC) $(CFLAGS) src/main.c src/hanoi.c -o $(BIN)
run: all
	python3 bridge/bridge.py
clean:
	rm -rf bin
