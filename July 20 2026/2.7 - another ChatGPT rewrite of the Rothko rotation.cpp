#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 800;

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Blurred Rectangle Texture",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    //--------------------------------------------------------
    // Rectangle settings
    //--------------------------------------------------------

    int blurSize = 12;

    int textureW = WINDOW_WIDTH;
    int textureH = 250;

    SDL_Texture* topTexture =
        SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            textureW,
            textureH);

    SDL_SetTextureBlendMode(topTexture, SDL_BLENDMODE_BLEND);

    //--------------------------------------------------------
    // Draw rectangle into the texture
    //--------------------------------------------------------

    SDL_SetRenderTarget(renderer, topTexture);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int topRectX1 = WINDOW_WIDTH / 20;
    int topRectY1 = blurSize;

    int topRectX2 = 19 * (WINDOW_WIDTH / 20);
    int topRectY2 = 200;

    int topRectRadius = 30;

    for (int i = 0; i < blurSize; i++)
    {
        roundedBoxRGBA(
            renderer,
            topRectX1 - i,
            topRectY1 - i,
            topRectX2 + i,
            topRectY2 + i,
            topRectRadius,
            238, 230, 113,
            255 - (i * 20));
    }

    SDL_SetRenderTarget(renderer, NULL);

    //--------------------------------------------------------

    bool running = true;
    SDL_Event event;

    double angle = -3.0;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;

            if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 87, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect destRect =
        {
            0,
            40,
            textureW,
            textureH
        };

        SDL_RenderCopyEx(
            renderer,
            topTexture,
            NULL,
            &destRect,
            angle,
            NULL,
            SDL_FLIP_NONE);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(topTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}