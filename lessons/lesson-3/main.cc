#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool init();
bool loadMedia();
void close();

SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
SDL_Surface* gSplash = nullptr;

int main(int argc, char** argv) {
  bool quit = false;
  SDL_Event e;
  
  if(init()) {
    if(loadMedia()) {      
      while(!quit) {
	while(SDL_PollEvent(&e) != 0) {
	  if(e.type == SDL_QUIT) {
	    quit = true;
	  }	  
	  SDL_BlitSurface(gSplash, NULL, gScreenSurface, NULL);
	  SDL_UpdateWindowSurface(gWindow);
	}
      }
    }
  }
  
  close();
  return 0;
}

bool init() {
  bool success = true;

  if(SDL_Init(SDL_INIT_VIDEO) < 0) {
    cerr << "Failed to initialize: " << SDL_GetError() << endl;
    success = false;
  }
  else {
    gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(gWindow == nullptr) {
      cerr << "Failed to create window: " << SDL_GetError() << endl;
      success = false;
    }
    else {
      gScreenSurface = SDL_GetWindowSurface(gWindow);
    }
  }

  return success;
}

bool loadMedia() {
  bool success = true;
  
  gSplash = SDL_LoadBMP("splash.bmp");
  if(gSplash == nullptr) {
    cerr << "Failed to load image: " << SDL_GetError() << endl;
    success = false;
  }
  
  return success;
}

void close() {
  SDL_FreeSurface(gSplash);
  SDL_DestroyWindow(gWindow);
  
  gSplash = nullptr;
  gWindow = nullptr;
  gScreenSurface = nullptr;

  SDL_Quit();
}
