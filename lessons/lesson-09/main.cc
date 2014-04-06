#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 638;
const int SCREEN_HEIGHT = 480;

enum class ERROR_CODES { INIT, MEDIA_LOAD };

bool init();
bool loadMedia();
void close();

SDL_Texture* loadTexture(const string& path);

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
SDL_Texture* gTexture = nullptr;

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

            SDL_RenderClear(gRenderer);

            SDL_Rect topLeftViewport;
            topLeftViewport.x = 0;
            topLeftViewport.y = 0;
            topLeftViewport.w = SCREEN_WIDTH / 2;
            topLeftViewport.h = SCREEN_HEIGHT / 2;
            SDL_RenderSetViewport(gRenderer, &topLeftViewport);
            SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

            SDL_Rect topRightViewport;
            topRightViewport.x = SCREEN_WIDTH / 2;
            topRightViewport.y = 0;
            topRightViewport.w = SCREEN_WIDTH / 2;
            topRightViewport.h = SCREEN_HEIGHT / 2;
            SDL_RenderSetViewport(gRenderer, &topRightViewport);
            SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

            SDL_Rect bottomViewport;
            bottomViewport.x = 0;
            bottomViewport.y = SCREEN_HEIGHT / 2;
            bottomViewport.w = SCREEN_WIDTH;
            bottomViewport.h = SCREEN_HEIGHT / 2;
            SDL_RenderSetViewport(gRenderer, &bottomViewport);
            SDL_RenderCopy(gRenderer, gTexture, nullptr, nullptr);

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

    SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

    if(!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
        cerr << "Error initializing SDL_image: " << IMG_GetError() << endl;
        return false;
    }

    return true;
}

bool loadMedia() {
    bool success = true;

    gTexture = loadTexture("../media/viewport.png");
    if(gTexture == nullptr) {
        success = false;
    }

    return success;
}

void close() {
    SDL_DestroyTexture(gTexture);
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gTexture = nullptr;
    gRenderer = nullptr;
    gWindow = nullptr;

    IMG_Quit();
    SDL_Quit();
}

SDL_Texture* loadTexture(const string& path) {
    SDL_Texture* texture = nullptr;
    SDL_Surface* image = IMG_Load(path.c_str());
    if(image == nullptr) {
        cerr << "Error loading surface (" << path << "): " << IMG_GetError() << endl;
    }
    else {
        texture = SDL_CreateTextureFromSurface(gRenderer, image);
        if(texture == nullptr) {
            cerr << "Error creating texture (" << path << "): " << SDL_GetError() << endl;
        }

        SDL_FreeSurface(image);
    }

    return texture;
}
