CC=clang
CFLAGS=-Wall -Wextra -ansi -pedantic -include raylib.h
LIBS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11

default: tetris-clone

OBJECTS = $(patsubst %.c, %.o, $(wildcard *.c))
HEADERS = $(wildcard *.h)

%.o: %.c $(HEADERS)
	$(CC) -c $< -o $@ $(CFLAGS)

.PRECIOUS: $(TARGET) $(OBJECTS)

tetris-clone: $(OBJECTS)
	$(CC) -o $@  $(OBJECTS) $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o
	rm -f tetris-clone