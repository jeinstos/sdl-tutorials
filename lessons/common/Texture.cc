#include "Texture.hh"
#include <SDL2/SDL_image.h>
#include <iostream>

using namespace std;

class TextureImpl {
    public:
        TextureImpl(SDL_Renderer* renderer) 
            : pRenderer_(renderer), pTexture_(nullptr), width_(0), height_(0) 
        {

        }

        ~TextureImpl() {
            free();
        }

        int getWidth() {
            return width_;
        }

        int getHeight() {
            return height_;
        }

        bool loadFromFile(const string& path) {
            free();
            SDL_Texture* newTexture = nullptr;
            SDL_Surface* loadedSurface = IMG_Load(path.c_str());
            if(loadedSurface == nullptr) {
                cerr << "Unable to load texture: " << IMG_GetError() << endl;
            }
            else {
                SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
                newTexture = SDL_CreateTextureFromSurface(pRenderer_, loadedSurface);
                if(newTexture == nullptr) {
                    cerr << "Unable to create texture: " << SDL_GetError() << endl;
                }
                else {
                    width_ = loadedSurface->w;
                    height_ = loadedSurface->h;
                }

                SDL_FreeSurface(loadedSurface);
            }

            pTexture_ = newTexture;
            return pTexture_ != nullptr;
        }

        void setColor(Uint8 red, Uint8 green, Uint8 blue) {
            SDL_SetTextureColorMod(pTexture_, red, green, blue);
        }

        void setBlendMode(SDL_BlendMode blend_mode) {
            SDL_SetTextureBlendMode(pTexture_, blend_mode);
        }

        void setAlpha(Uint8 alpha) {
            SDL_SetTextureAlphaMod(pTexture_, alpha);
        }

        void render(int x, int y, SDL_Rect* clip = nullptr) {
            SDL_Rect renderQuad = { x, y, width_, height_ };
            if(clip != nullptr) {
                renderQuad.w = clip->w;
                renderQuad.h = clip->h;
            }

            SDL_RenderCopy(pRenderer_, pTexture_, clip, &renderQuad);
        }

        void free() {
            if(pTexture_ != nullptr) {
                SDL_DestroyTexture(pTexture_);
                pTexture_ = nullptr;
                width_ = 0;
                height_ = 0;
            }
        }

    private:
        SDL_Renderer* pRenderer_;
        SDL_Texture* pTexture_;
        int width_;
        int height_;
};

Texture::Texture(SDL_Renderer* renderer)
    : pImpl(new TextureImpl(renderer))
{

};

Texture::~Texture() {
    delete pImpl;
};

bool Texture::loadFromFile(const string& path) {
    return pImpl->loadFromFile(path);
};

int Texture::getWidth() {
    return pImpl->getWidth();
};

int Texture::getHeight() {
    return pImpl->getHeight();
};

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue) {
    pImpl->setColor(red, green, blue);
};

void Texture::setBlendMode(SDL_BlendMode blend_mode) {
    pImpl->setBlendMode(blend_mode);
};

void Texture::setAlpha(Uint8 alpha) {
    pImpl->setAlpha(alpha);
};

void Texture::render(int x, int y, SDL_Rect* clip) {
    pImpl->render(x, y, clip);
};

void Texture::free() {
    pImpl->free();
};
