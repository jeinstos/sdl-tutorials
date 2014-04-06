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

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

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

         SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
         SDL_RenderClear(gRenderer);

         // Fill a rectangle
         SDL_Rect fillRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
         SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
         SDL_RenderFillRect(gRenderer, &fillRect);

         // Draw an empty rectangle
         SDL_Rect outlineRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
         SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
         SDL_RenderDrawRect(gRenderer, &outlineRect);

         // Blue Horizontal line
         SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
         SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

         SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
         for(int i = 0; i < SCREEN_HEIGHT; i += 4) {
            SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i);
         }

         SDL_RenderPresent(gRenderer);
      }
   }

   close();
   return 0;
}

bool init() {
   if(SDL_Init(SDL_INIT_VIDEO) < 0) {
      cerr << "Failed to initialize: " << SDL_GetError() << endl;
      return false;
   }

   gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
   if(gWindow == nullptr) {
      cerr << "Failed to create window: " << SDL_GetError() << endl;
      return false;
   }

   gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
   if(gRenderer == nullptr) {
      cerr << "Error creating renderer: " << SDL_GetError() << endl;
      return false;
   }

   return true;
}

bool loadMedia() {
   bool success = true;
   return success;
}

void close() {
   SDL_DestroyRenderer(gRenderer);
   SDL_DestroyWindow(gWindow);

   gRenderer = nullptr;
   gWindow = nullptr;

   IMG_Quit();
   SDL_Quit();
}
