COMPILER = g++
SDL_LIB = -L/usr/local/lib -lSDL2 -Wl,-rpath=/usr/local/lib
SDL_INCLUDE = -I/usr/local/include

COMPILER_FLAGS = -Wall -c -g -std=c++11 $(SDL_INCLUDE)
LD_FLAGS = $(SDL_LIB)

OUTPUT = lesson1.out

all: $(OUTPUT)

$(OUTPUT): main.o
	$(COMPILER) $< $(LD_FLAGS) -o $@

main.o: main.cc
	$(COMPILER) $(COMPILER_FLAGS) $< -o $@

clean:
	rm *.o && rm $(OUTPUT)