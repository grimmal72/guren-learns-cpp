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
    if (!backgroundTexture) {
        SDL_Log("Failed to load texture: %s", IMG_GetError());
        return 1;
    }

    // Since the PNG has transparency, make sure blending is on
    SDL_SetTextureBlendMode(backgroundTexture, SDL_BLENDMODE_BLEND);

    int textureW = 800; // your image width
    int textureH = 600; // your image height

    float scrollX = 0.0f;
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
        scrollX += scrollSpeed * deltaTime;
        if (scrollX >= textureW) {
            scrollX -= textureW; // wrap back around, keeps float from growing forever
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Draw two copies side-by-side to cover the wrap-around gap
        SDL_Rect dest1 = { -(int)scrollX, 0, textureW, textureH };
        SDL_Rect dest2 = { -(int)scrollX + textureW, 0, textureW, textureH };

        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &dest1);
        SDL_RenderCopy(renderer, backgroundTexture, nullptr, &dest2);

        // ... draw your foreground/sprites here ...

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}