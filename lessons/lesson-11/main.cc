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

            SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0x00, 0xFF, 0xFF));
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

        void render(int x, int y, SDL_Rect* clip = nullptr) {
            SDL_Rect renderQuad = {x, y, mWidth, mHeight};

            if(clip != nullptr) {
                renderQuad.w = clip->w;
                renderQuad.h = clip->h;
            }

            SDL_RenderCopy(gRenderer, mTexture, clip, &renderQuad);
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

SDL_Rect gSpriteClips[4];
LTexture gSpriteSheetTexture;

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

            gSpriteSheetTexture.render(0, 0, &gSpriteClips[0]);
            gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[1].w, 0, &gSpriteClips[1]);
            gSpriteSheetTexture.render(0, SCREEN_HEIGHT - gSpriteClips[2].h, &gSpriteClips[2]);
            gSpriteSheetTexture.render(SCREEN_WIDTH - gSpriteClips[3].w, SCREEN_HEIGHT - gSpriteClips[3].h, &gSpriteClips[3]);

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

    if(!gSpriteSheetTexture.loadFromFile("../media/dots.png")) {
        cerr << "Failed to load sprite sheet texture." << endl;
        success = false;
    }
    else {
        // Top left sprite
        gSpriteClips[0].x = 0;
        gSpriteClips[0].y = 0;
        gSpriteClips[0].w = 100;
        gSpriteClips[0].h = 100;
        // Top Right
        gSpriteClips[1].x = 100;
        gSpriteClips[1].y = 0;
        gSpriteClips[1].w = 100;
        gSpriteClips[1].h = 100;
        // Bottom left
        gSpriteClips[2].x = 0;
        gSpriteClips[2].y = 100;
        gSpriteClips[2].w = 100;
        gSpriteClips[2].h = 100;
        // Bottom right
        gSpriteClips[3].x = 100;
        gSpriteClips[3].y = 100;
        gSpriteClips[3].w = 100;
        gSpriteClips[3].h = 100;
    }

    return success;
}

void close() {
    gSpriteSheetTexture.free();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    gRenderer = nullptr;
    gWindow = nullptr;

    IMG_Quit();
    SDL_Quit();
}
