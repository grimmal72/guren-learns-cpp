#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

const int WINDOW_W = 800;
const int WINDOW_H = 600;

// The clipped box — smaller than the window, positioned wherever you like
SDL_Rect clippedRect = { 150, 100, 500, 400 }; // x, y, w, h

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cout << "IMG_Init failed: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Clipped Wallpaper Box",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr)
    {
        std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == nullptr)
    {
        std::cout << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // ---- Load the wallpaper PNG ----
    SDL_Texture* wallpaperTexture = IMG_LoadTexture(renderer, "wallpaper.png"); // Replace with your actual path
    if (wallpaperTexture == nullptr)
    {
        std::cout << "IMG_LoadTexture failed: " << IMG_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    int textureW = 0, textureH = 0;
    SDL_QueryTexture(wallpaperTexture, nullptr, nullptr, &textureW, &textureH);

    // Destination rect: unscaled image, anchored at the clip box's upper-left corner
    SDL_Rect wallpaperDest = { clippedRect.x, clippedRect.y, textureW, textureH };

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
            {
                running = false;
            }
        }

        // Background so the clip box is visually obvious
        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // ---- Apply the clip, draw the wallpaper, remove the clip ----
        SDL_RenderSetClipRect(renderer, &clippedRect);

        SDL_RenderCopy(renderer, wallpaperTexture, nullptr, &wallpaperDest);

        SDL_RenderSetClipRect(renderer, nullptr);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(wallpaperTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}