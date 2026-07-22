#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 800;

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Render-To-Texture Rotation",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        std::cout << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    //--------------------------------------------------------
    // Create a texture that will hold one blurred rectangle.
    //--------------------------------------------------------

    int textureW = 620;
    int textureH = 240;

    SDL_Texture* topTexture =
        SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_RGBA8888,
            SDL_TEXTUREACCESS_TARGET,
            textureW,
            textureH);

    SDL_SetTextureBlendMode(topTexture, SDL_BLENDMODE_BLEND);

    //--------------------------------------------------------
    // Draw into the texture ONCE.
    //--------------------------------------------------------

    SDL_SetRenderTarget(renderer, topTexture);

    // Transparent background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    int x1 = 12;
    int y1 = 12;
    int x2 = textureW - 12;
    int y2 = 200;
    int radius = 30;

    for (int i = 0; i < 12; i++)
    {
        roundedBoxRGBA(
            renderer,
            x1 - i,
            y1 - i,
            x2 + i,
            y2 + i,
            radius,
            238,
            230,
            113,
            255 - i * 20);
    }

    // Return to rendering to the window.
    SDL_SetRenderTarget(renderer, NULL);

    //--------------------------------------------------------

    bool running = true;
    SDL_Event event;

    double angle = -2.5;

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

        // Background
        SDL_SetRenderDrawColor(renderer, 87, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect destRect;

        destRect.x = 10;
        destRect.y = 40;
        destRect.w = textureW;
        destRect.h = textureH;

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