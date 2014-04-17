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

const int WALKING_ANIMATION_FRAMES = 4;
SDL_Rect gSpriteClips[WALKING_ANIMATION_FRAMES];
Texture* gSpriteSheetTexture = nullptr;

bool init();
bool loadMedia();
void close();

int main() {
    bool quit = false;
    SDL_Event e;
    int frame = 0;

    if(!init())
        return -1;

    if(!loadMedia())
        return -1;

    while(!quit) {
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        SDL_Rect* currentClip = &gSpriteClips[frame / 12];
        gSpriteSheetTexture->render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip);

        SDL_RenderPresent(gRenderer);

        ++frame;
        if(frame / 12 >= WALKING_ANIMATION_FRAMES)
            frame = 0;
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

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(gRenderer == nullptr) {
        cerr << "Error creating renderer: " << SDL_GetError() << endl;
        return false;
    }

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    gSpriteSheetTexture = new Texture(gRenderer);

    if(!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
        cerr << "Error initializing SDL_image: " << IMG_GetError() << endl;
        return false;
    }

    return true;
}

bool loadMedia() {
    bool success = true;

    if(!gSpriteSheetTexture->loadFromFile("../media/animation.png")) {
        cerr << "Failed to load animation texture!" << endl;
        success = false;
    }
    else {
        gSpriteClips[0].x = 0;
        gSpriteClips[0].y = 0;
        gSpriteClips[0].w = 64;
        gSpriteClips[0].h = 205;
        
        gSpriteClips[1].x = 64;
        gSpriteClips[1].y = 0;
        gSpriteClips[1].w = 64;
        gSpriteClips[1].h = 205;
        
        gSpriteClips[2].x = 128;
        gSpriteClips[2].y = 0;
        gSpriteClips[2].w = 64;
        gSpriteClips[2].h = 205;
        
        gSpriteClips[3].x = 196;
        gSpriteClips[3].y = 0;
        gSpriteClips[3].w = 64;
        gSpriteClips[3].h = 205;
    }

    return success;
}

void close() {
    delete gSpriteSheetTexture;

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gRenderer = nullptr;
    gWindow = nullptr;
    gSpriteSheetTexture = nullptr;

    IMG_Quit();
    SDL_Quit();
}
