CC=g++
MW=x86_64-w64-mingw32-g++
CFLAGS=-Wall -Wextra -Werror -std=c++17
OUT=program

run:
	make build
	./$(OUT)

clean:
	rm -f $(OUT)

build: main.cpp main.h
	$(CC) $(CFLAGS) -o $(OUT) main.cpp main.h -lSDL2 -lSDL2_ttf

window: main.cpp main.h
	$(MW) $(CFLAGS) -o $(OUT) main.cpp main.h -lSDL2 -lSDL2_ttf

wasm: 
#	mkdir wasm
	emcc main.cpp -o index.html -s USE_SDL=2 -s USE_SDL_TTF=2 --embed-file=./FSEX302.ttf
#	mv index.html wasm 
#	mv index.js wasm
#	mv index.wasm wasm
