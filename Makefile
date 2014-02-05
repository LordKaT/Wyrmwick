# SDL_CCFLAGS=$(shell sdl2-config --cflags)
SDL_CCFLAGS=-I/usr/local/include/SDL2 -D_REENTRANT
# SDL_LDFLAGS=$(shell sdl2-config --libs)
SDL_LDFLAGS=-L/usr/local/lib -Wl,-rpath,/usr/local/lib -lSDL2 -lpthread
LUA_LDFLAGS=-Lsrc/lua-5.2.3/ -llua

CC=g++
LD=g++
CCFLAGS=$(SDL_CCFLAGS) -std=c++0x -Wall -g

LDFLAGS=-ldl $(LUA_LDFLAGS) $(SDL_LDFLAGS) 

INCLUDES=src/include.h src/define.h src/structs.h src/globals.h src/debug.h src/script.h src/screen.h src/sdl.h \
	src/font.h src/menu.h

OBJS=bin/debug.o bin/font.o bin/globals.o bin/items.o bin/main.o bin/menu.o bin/screen.o \
	bin/script.o bin/sdl.o bin/utils.o

.PHONY: all clean bread

all: bread

bread: bin/wyrmwick

bin/wyrmwick: bin/main.o $(OBJS)
	$(LD) -o bin/wyrmwick $(OBJS) $(LDFLAGS) 

bin/%.o: src/%.cpp src/%.h $(INCLUDES)
	$(CC) $(CCFLAGS) -o $@ -c $<

bin/main.o: src/main.cpp $(INCLUDES)
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm bin/wyrmwick bin/*.o
