SDL_CCFLAGS=$(shell sdl2-config --cflags)
SDL_LDFLAGS=$(shell sdl2-config --libs) -lSDL2_mixer
LUA_LDFLAGS=-Lsrc/lua-5.2.3/ -llua

CC=g++
LD=g++
CCFLAGS=$(SDL_CCFLAGS) -std=c++0x -Wall -g

LDFLAGS=-ldl $(LUA_LDFLAGS) $(SDL_LDFLAGS) 

SOURCES=$(wildcard src/*.cpp)
INCLUDES=$(wildcard src/*.h)
OBJS=$(SOURCES:src/%.cpp=bin/%.o)


.PHONY: all clean

all: bin/wyrmwick

bin/wyrmwick: $(OBJS)
	$(LD) -o bin/wyrmwick $(OBJS) $(LDFLAGS) 

bin/%.o: src/%.cpp $(INCLUDES)
	$(CC) $(CCFLAGS) -o $@ -c $<

clean:
	rm -f bin/wyrmwick bin/*.o
