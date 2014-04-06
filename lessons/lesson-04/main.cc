#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

enum class ERROR_CODES { INIT, MEDIA_LOAD };
enum class SURFACE_STATE { DEFAULT, UP, LEFT, RIGHT, DOWN, TOTAL };

bool init();
bool loadMedia();
void close();

SDL_Surface* loadSurface(const string& path);

SDL_Window* gWindow = nullptr;
SDL_Surface* gScreenSurface = nullptr;
SDL_Surface* gCurrentSurface = nullptr;
SDL_Surface* gSurfaces[(int)SURFACE_STATE::TOTAL];

int main(int argc, char** argv) {
  bool quit = false;
  SDL_Event e;

  if(!init())
    return (int)ERROR_CODES::INIT;
  
  if(!loadMedia())
    return (int)ERROR_CODES::MEDIA_LOAD;
  
  gCurrentSurface = gSurfaces[(int)SURFACE_STATE::DEFAULT];

  while(!quit) {
    while(SDL_PollEvent(&e) != 0) {
      if(e.type == SDL_QUIT) {
	quit = true;
      }
      else if(e.type == SDL_KEYDOWN) {
	switch(e.key.keysym.sym) {
	case SDLK_UP:
	  gCurrentSurface = gSurfaces[(int)SURFACE_STATE::UP];
	  break;
	case SDLK_DOWN:
	  gCurrentSurface = gSurfaces[(int)SURFACE_STATE::DOWN];
	  break;
	case SDLK_LEFT:
	  gCurrentSurface = gSurfaces[(int)SURFACE_STATE::LEFT];
	  break;
	case SDLK_RIGHT:
	  gCurrentSurface = gSurfaces[(int)SURFACE_STATE::RIGHT];
	  break;
	default:
	  gCurrentSurface = gSurfaces[(int)SURFACE_STATE::DEFAULT];
	  break;
	}
      }
      
      SDL_BlitSurface(gCurrentSurface, NULL, gScreenSurface, NULL);
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
      gScreenSurface = SDL_GetWindowSurface(gWindow);
    }
  }

  return success;
}

bool loadMedia() {
  bool success = true;
  const string prefix = "../media/";
  
  gSurfaces[(int)SURFACE_STATE::DEFAULT] = loadSurface(prefix + "default.bmp");
  gSurfaces[(int)SURFACE_STATE::UP]      = loadSurface(prefix + "up.bmp");
  gSurfaces[(int)SURFACE_STATE::DOWN]    = loadSurface(prefix + "down.bmp");
  gSurfaces[(int)SURFACE_STATE::LEFT]    = loadSurface(prefix + "left.bmp");
  gSurfaces[(int)SURFACE_STATE::RIGHT]   = loadSurface(prefix + "right.bmp");

  if(gSurfaces[(int)SURFACE_STATE::DEFAULT] == nullptr || gSurfaces[(int)SURFACE_STATE::UP] == nullptr   || 
     gSurfaces[(int)SURFACE_STATE::RIGHT] == nullptr   || gSurfaces[(int)SURFACE_STATE::LEFT] == nullptr ||
     gSurfaces[(int)SURFACE_STATE::DOWN] == nullptr) {
    success = false;
  }
  
  return success;
}

void close() {
  for(int i = 0; i < (int)SURFACE_STATE::TOTAL; i++) {
    SDL_FreeSurface(gSurfaces[i]);
    gSurfaces[i] = nullptr;
  }
  SDL_DestroyWindow(gWindow);
  
  gWindow = nullptr;
  gScreenSurface = nullptr;

  SDL_Quit();
}

SDL_Surface* loadSurface(const string& path) {
  SDL_Surface* image = SDL_LoadBMP(path.c_str());
  if(image == nullptr) {
    cerr << "Error loading surface (" << path << "): " << SDL_GetError() << endl;
  }
  
  return image;
}
