#include <SDL2/SDL.h>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char** argv) {
  SDL_Window* window = nullptr;
  SDL_Surface* screenSurface = nullptr;
  
  if(SDL_Init(SDL_INIT_VIDEO) < 0)
    cerr << "SDL error: " << SDL_GetError() << endl;
  else {
    window = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, 
			      SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(window == nullptr)
      cerr << "SDL error: " << SDL_GetError() << endl;
    else {
      screenSurface = SDL_GetWindowSurface(window);
      SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
      SDL_UpdateWindowSurface(window);
      SDL_Delay(2000);
    }
  }
  
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
