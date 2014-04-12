#ifndef TEXTURE_HH
#define TEXTURE_HH

#include <SDL2/SDL.h>
#include <string>

class TextureImpl;

class Texture {
    public:
        Texture(SDL_Renderer* renderer);
        ~Texture();

        bool loadFromFile(const std::string& path);
        void setColor(Uint8 red, Uint8 green, Uint8 blue);
        void setBlendMode(SDL_BlendMode blend_mode);
        void setAlpha(Uint8 alpha);
        void render(int x, int y, SDL_Rect* clip = nullptr);
        void free();

        int getWidth();
        int getHeight();

    private:
        TextureImpl* pImpl;
};

#endif // TEXTURE_HH
