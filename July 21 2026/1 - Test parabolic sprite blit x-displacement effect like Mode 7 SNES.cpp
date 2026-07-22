// PC-Engine style "rotating tube" background effect.
// Loads image.png (expected 640x480) and re-blits it one scanline at a time,
// shifting each row horizontally according to a sine wave whose phase
// advances over time -- this is the same trick used for scanline-based
// pseudo-rotation effects on 16-bit hardware.
//
// Place image.png (640x480) in the working directory next to the executable.

#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <cstdio>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    int imgFlags = IMG_INIT_PNG;

    if (!(IMG_Init(imgFlags) & imgFlags)) {
        SDL_Log("IMG_Init failed: %s", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Raster Curve Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture* bgTexture = IMG_LoadTexture(renderer, "image.png");

    if (!bgTexture) {
        SDL_Log("IMG_LoadTexture failed: %s", IMG_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    int textureW = 0;
    int textureH = 0;
    
    SDL_QueryTexture(bgTexture, nullptr, nullptr, &textureW, &textureH);
    
    // Where to draw the top-left of the (unshifted) image in the window.
    const int destX = (WINDOW_WIDTH - textureW) / 2;
    const int destY = (WINDOW_HEIGHT - textureH) / 2;

    bool running = true;
    SDL_Event event;

    Uint64 startTicks = SDL_GetPerformanceCounter();
    Uint64 freq = SDL_GetPerformanceFrequency();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;
        }

        double time = (double)(SDL_GetPerformanceCounter() - startTicks) / (double)freq;
        std::cout << time << std::endl; // Optionally comment this out, but it helps to note that time counts seconds.

        SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
        SDL_RenderClear(renderer);

        // --- The raster effect itself ---
        // Each scanline of the source texture is copied to the screen with
        // its own horizontal offset. The offset is driven by a sine wave
        // whose phase depends on row index (for the curve shape) and time
        // (for the rotation/animation).
        float centerY = textureH / 2.0f;
        float amplitude = 60.0f;   // how far rows swing left/right, in px
        float frequency = 0.015f;  // how tight the curve is, per pixel row
        float speed = 1.5f;        // radians/sec of phase scroll -> "rotation"

        for (int y = 0; y < textureH; y++) {
            float phase = (y - centerY) * frequency + (float)time * speed;
            int offsetX = (int)(sinf(phase) * amplitude);

            SDL_Rect srcRect{ 0, y, textureW, 1 };
            SDL_Rect destRect{ destX + offsetX, destY + y, textureW, 1 };
            SDL_RenderCopy(renderer, bgTexture, &srcRect, &destRect);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(bgTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}