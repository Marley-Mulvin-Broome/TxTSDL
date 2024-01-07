CC = gcc
CFLAGS = -Wall
DEBUGFLAGS = -g
SRCS = $(wildcard src/*.c src/ds/*.c src/tui/*.c src/font/*.c)
OBJS = $(SRCS:.c=.o)

build: $(OBJS)
	$(CC) $(CFLAGS) -o game $^ -lSDL2

debug-build: DEBUGFLAGS += -g
debug-build: DEBUGFLAGS += -O0
debug-build: build

%.o: %.c
	$(CC) $(CFLAGS) $(DEBUGFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS) game

all: clean build

run: build
	./game

debug: clean debug-build
	gdb game