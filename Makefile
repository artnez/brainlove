CFLAGS=-Wall -std=gnu99 

all: build

debug: CFLAGS += -DDEBUG -g
debug: brainlove

build: CFLAGS += -O3
build: brainlove

brainlove: src/*.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f brainlove

.PHONY: all debug build clean
