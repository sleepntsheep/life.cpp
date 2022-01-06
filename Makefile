CC=g++
CFLAGS=-Wall -Wextra -Werror -std=c++17
OUT=program

run:
	make build
	./$(OUT)

clean:
	rm -f $(OUT)

build: main.cpp main.h
	$(CC) $(CFLAGS) -o $(OUT) main.cpp main.h -lSDL2 -lSDL2_ttf
