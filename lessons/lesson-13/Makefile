COMPILER = clang++
SDL_LIB = -lSDL2 -lSDL2_image

COMPILER_FLAGS = -Wall -c -g -std=c++11 -I "/home/jonathan/repos/sdl-tutorials/lessons/common"
LD_FLAGS =  $(SDL_LIB)

OUTPUT = lesson13.out

all: $(OUTPUT)

$(OUTPUT): main.o texture.o
	$(COMPILER) main.o texture.o $(LD_FLAGS) -o $@

texture.o: ../common/Texture.cc
	$(COMPILER) $(COMPILER_FLAGS) $< -o $@

main.o: main.cc
	$(COMPILER) $(COMPILER_FLAGS) $< -o $@

run: all
	./$(OUTPUT)

clean:
	rm *.o && rm $(OUTPUT)
