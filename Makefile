CC = gcc
SRCS = $(wildcard src/*.c src/ds/*.c src/tui/*.c)
OBJS = $(SRCS:.c=.o)

build: $(OBJS)
	$(CC) -o game $^ -lSDL2

%.o: %.c
	$(CC) -c $< -o $@

clean:
	$(RM) $(OBJS) game

all: clean build

run: build
	./game