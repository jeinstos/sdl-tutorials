#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum class ERROR_CODES { INIT, MEDIA_LOAD };

bool init();
bool loadMedia();
void close();

SDL_Surface* loadOptimizedSurface(const string& path);

SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
SDL_Surface* gCurrentSurface = nullptr;

int main(int argc, char** argv) {
  bool quit = false;
  SDL_Event e;

  if(!init())
    return (int)ERROR_CODES::INIT;
  
  if(!loadMedia())
    return (int)ERROR_CODES::MEDIA_LOAD;

  while(!quit) {
    while(SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT) {
	quit = true;
      }

      SDL_Rect stretchRect;
      stretchRect.x = 0;
      stretchRect.y = 0;
      stretchRect.w = SCREEN_WIDTH;
      stretchRect.h = SCREEN_HEIGHT;

      SDL_BlitScaled(gCurrentSurface, NULL, gScreenSurface, &stretchRect);
      SDL_UpdateWindowSurface(gWindow);
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
      if(!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
	cerr << "Error initializing SDL_image: " << IMG_GetError() << endl;
	success = false;
      }
      else {
	gScreenSurface = SDL_GetWindowSurface(gWindow);
      }
    }
  }

  return success;
}

bool loadMedia() {
  bool success = true;
  
  gCurrentSurface = loadOptimizedSurface("../media/mypng.png");
  if(gCurrentSurface == nullptr) {
    success = false;
  }
  
  return success;
}

void close() {

  SDL_FreeSurface(gCurrentSurface);
  SDL_DestroyWindow(gWindow);
  
  gWindow = nullptr;
  gScreenSurface = nullptr;
  gCurrentSurface = nullptr;

  IMG_Quit();
  SDL_Quit();
}

SDL_Surface* loadOptimizedSurface(const string& path) {
  SDL_Surface* optimizedImage = nullptr;
  SDL_Surface* image = IMG_Load(path.c_str());
  if(image == nullptr) {
    cerr << "Error loading surface (" << path << "): " << IMG_GetError() << endl;
  }
  else {
    optimizedImage = SDL_ConvertSurface(image, gScreenSurface->format, 0);
    if(optimizedImage == nullptr) {
      cerr << "Error optimizing image (" << path << "): " << SDL_GetError() << endl;
    }
    
    SDL_FreeSurface(image);
  }
  
  return optimizedImage;
}
