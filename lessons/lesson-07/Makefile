COMPILER = g++
SDL_LIB = -lSDL2 -lSDL2_image

COMPILER_FLAGS = -Wall -c -g -std=c++11
LD_FLAGS = $(SDL_LIB)

OUTPUT = lesson7.out

all: $(OUTPUT)

$(OUTPUT): main.o
	$(COMPILER) $< $(LD_FLAGS) -o $@

main.o: main.cc
	$(COMPILER) $(COMPILER_FLAGS) $< -o $@

clean:
	rm *.o && rm $(OUTPUT)