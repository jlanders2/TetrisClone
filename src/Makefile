CC=clang
CFLAGS=-Wall -Wextra -include raylib.h
LIBS=-lraylib -lGL -lm -lpthread -ldl -lrt -lX11

tetris-clone: main.c manager.c renderer.c app.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f ./tetris-clone
	rm -f ./*.o
	rm -f ./*.gch
