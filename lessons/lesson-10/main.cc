#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

using namespace std;

const int SCREEN_WIDTH = 638;
const int SCREEN_HEIGHT = 480;

enum class ERROR_CODES { INIT, MEDIA_LOAD };

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

class LTexture {
    public:
        LTexture() : mTexture(nullptr), mWidth(0), mHeight(0) {

        };

        ~LTexture() {
            free();
        };

        bool loadFromFile(const string& path) {
            free();
            SDL_Texture* newTexture = nullptr;

            SDL_Surface* loadedSurface = IMG_Load(path.c_str());
            if(loadedSurface == nullptr) {
                cerr << "Unable to load image (" << path << "): " << IMG_GetError() << endl;
                return false;
            }

            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0xFF, 0xFF, 0xFF));
            newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
            if(newTexture == nullptr) {
                cerr << "Unable to create texture (" << path << "): " << SDL_GetError() << endl;
            }
            else {
                mWidth = loadedSurface->w;
                mHeight = loadedSurface->h;
            }

            SDL_FreeSurface(loadedSurface);
            mTexture = newTexture;
            return mTexture != nullptr;
        };

        void free() {
            if(mTexture != nullptr) {
                SDL_DestroyTexture(mTexture);
                mTexture = nullptr;
                mWidth = 0;
                mHeight = 0;
            }
        };

        void render(int x, int y) {
            SDL_Rect renderQuad = {x, y, mWidth, mHeight};
            SDL_RenderCopy(gRenderer, mTexture, nullptr, &renderQuad);
        };

        int getWidth() {
            return mWidth;
        };

        int getHeight() {
            return mHeight;
        };

    private:
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;
};

bool init();
bool loadMedia();
void close();

LTexture gStickmanTexture;
LTexture gBackgroundTexture;

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

            gBackgroundTexture.render(0, 0);
            gStickmanTexture.render(240, 190);

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

    if(!IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) {
        cerr << "Error initializing SDL_image: " << IMG_GetError() << endl;
        return false;
    }

    return true;
}

bool loadMedia() {
    bool success = true;

    if(!gStickmanTexture.loadFromFile("../media/stickman.png")) {
        cerr << "Unable to load Stick Man texture." << endl;
        success = false;
    }

    if(!gBackgroundTexture.loadFromFile("../media/background.png")) {
        cerr << "Unable to load background texture." << endl;
        success = false;
    }

    return success;
}

void close() {
    gStickmanTexture.free();
    gBackgroundTexture.free();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gRenderer = nullptr;
    gWindow = nullptr;

    IMG_Quit();
    SDL_Quit();
}
