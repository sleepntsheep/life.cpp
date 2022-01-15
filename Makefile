CC=g++
MW=i686-w64-mingw32-g++
CFLAGS=-std=c++17 -Wl, -Bstatic -static-libgcc -static-libstdc++ -lstdc++ -lpthread #-Wl,-Bdynamic -Werror 
CFLAGSW=-std=c++17 -static-libgcc -static-libstdc++ -lstdc++ -lwinpthread #-Wl,-Bdynamic -Werror 
OUT=program

run:
	make build
	./$(OUT)

clean:
	rm -f $(OUT)

build: main.cc main.h
	$(CC) $(CFLAGS) -o $(OUT) main.cc main.h -lSDL2 -lSDL2_ttf

window: main.cc main.h
	$(MW) $(CFLAGSW) -o $(OUT) main.cc main.h -lSDL2 -lSDL2_ttf

wasm:
#	mkdir wasm
	emcc main.cc -o index.html -s USE_SDL=2 -s USE_SDL_TTF=2 --embed-file=./FSEX302.ttf
#	mv index.html wasm 
#	mv index.js wasm
#	mv index.wasm wasm
