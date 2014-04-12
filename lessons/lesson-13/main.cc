#include "Texture.hh"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <string>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 638;
const int SCREEN_HEIGHT = 480;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
Texture* gModulatedTexture = nullptr;
Texture* gBackgroundTexture = nullptr;

bool init();
bool loadMedia();
void close();

int main() {
    bool quit = false;
    SDL_Event e;

    if(!init())
        return -1;

    if(!loadMedia())
        return -1;

    Uint8 a = 255;

    while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
            else if(e.type == SDL_KEYDOWN) {
                switch(e.key.keysym.sym) {
                    case SDLK_w:
                        if(a + 32 > 255) {
                            a = 255;
                        }
                        else {
                            a += 32;
                        }
                        break;
                    case SDLK_s:
                        if(a - 32 < 0) {
                            a = 0;
                        }
                        else {
                            a -= 32;
                        }
                        break;
                    default:
                        break;
                }
            }
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            gBackgroundTexture->render(0, 0);
            gModulatedTexture->setAlpha(a);
            gModulatedTexture->render(0, 0);

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

    if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        cerr << "Warning: Linear texture filtering not enabled." << endl;

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

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    gModulatedTexture = new Texture(gRenderer);
    gBackgroundTexture = new Texture(gRenderer);

    if(!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
        cerr << "Error initializing SDL_image: " << IMG_GetError() << endl;
        return false;
    }

    return true;
}

bool loadMedia() {
    bool success = true;

    if(!gModulatedTexture->loadFromFile("../media/fadeout.png")) {
        cerr << "Failed to load modulated texture!" << endl;
        success = false;
    }

    if(!gBackgroundTexture->loadFromFile("../media/fadein.png")) {
        cerr << "Failed to load background texture!" << endl;
        success = false;
    }

    return success;
}

void close() {
    delete gModulatedTexture;
    delete gBackgroundTexture;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gRenderer = nullptr;
    gWindow = nullptr;
    gModulatedTexture = nullptr;
    gBackgroundTexture = nullptr;

    IMG_Quit();
    SDL_Quit();
}
