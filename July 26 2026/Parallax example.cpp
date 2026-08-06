#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

const int WINDOW_W = 800;
const int WINDOW_H = 600;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow(
        "Scrolling BG",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    // Load the background as a texture
    SDL_Texture* backgroundTexture = IMG_LoadTexture(renderer, "parallax-background.png");
    SDL_Texture* backgroundTexture2 = IMG_LoadTexture(renderer, "parallax-background-2.png");

    if (!(backgroundTexture || backgroundTexture2)) {
        SDL_Log("Failed to load texture: %s", IMG_GetError());
        return 1;
    }

    // Since the PNGs have transparency, make sure blending is on
    SDL_SetTextureBlendMode(backgroundTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(backgroundTexture2, SDL_BLENDMODE_BLEND);

    int background1TextureW = 800; // your image width
    int background1TextureH = 600; // your image height

    int background2TextureW = 800; // your image width
    int background2TextureH = 600; // your image height

    float background1ScrollX = 0.0f;
    float background2ScrollX = 0.0f;
    float scrollSpeed = 60.0f; // pixels per second

    bool running = true;
    Uint64 lastTick = SDL_GetPerformanceCounter();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        // Delta time so scroll speed is frame-rate independent
        Uint64 now = SDL_GetPerformanceCounter();
        float deltaTime = (float)(now - lastTick) / SDL_GetPerformanceFrequency();
        std::cout << deltaTime << std::endl; // Optional line to see the frame rate in the console
        lastTick = now;

        // Increment and wrap
        background1ScrollX += scrollSpeed * deltaTime;
        if (background1ScrollX >= background1TextureW) {
            background1ScrollX -= background1TextureW; // wrap back around, keeps float from growing forever
        }

        background2ScrollX += scrollSpeed * deltaTime * 1.3;
        if (background2ScrollX >= background2TextureW) {
            background2ScrollX -= background2TextureW; // wrap back around
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw two copies side-by-side to cover the wrap-around gap
        SDL_Rect background1Dest1 = { -(int)background1ScrollX, 0, background1TextureW, background1TextureH };
        SDL_Rect background1Dest2 = { -(int)background1ScrollX + background1TextureW, 0, background1TextureW, background1TextureH };

        SDL_Rect background2Dest1 = { -(int)(background2ScrollX), 0, background2TextureW, background2TextureH };
        SDL_Rect background2Dest2 = { -(int)(background2ScrollX) + background2TextureW, 0, background2TextureW, background2TextureH };

        
        SDL_RenderCopy(renderer, backgroundTexture2, nullptr, &background2Dest1);
        SDL_RenderCopy(renderer, backgroundTexture2, nullptr, &background2Dest2);

        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &background1Dest1);
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &background1Dest2);

        // ... draw your foreground/sprites here ...

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(backgroundTexture2);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}