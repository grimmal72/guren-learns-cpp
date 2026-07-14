#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Blend Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1000, 600,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        return 1;
    }

    // Load background
    SDL_Surface* bgSurface = IMG_Load("piece1.png");
    if (!bgSurface) {
        printf("Failed to load piece1.png: %s\n", IMG_GetError());
        return 1;
    }
    SDL_Texture* bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
    SDL_FreeSurface(bgSurface);

    // Load subject (with transparency)
    SDL_Surface* fgSurface = IMG_Load("piece2.png");
    if (!fgSurface) {
        printf("Failed to load piece2.png: %s\n", IMG_GetError());
        return 1;
    }
    SDL_Texture* fgTexture = SDL_CreateTextureFromSurface(renderer, fgSurface);
    SDL_FreeSurface(fgSurface);

    // Load subject 2
    SDL_Surface* subject2Surface = IMG_Load("piece3.png");
    if (!subject2Surface) {
        printf("Failed to load piece3.png: %s\n", IMG_GetError());
        return 1;
    }
    SDL_Texture* subject2Texture = SDL_CreateTextureFromSurface(renderer, subject2Surface);
    SDL_FreeSurface(subject2Surface);

    // Load subject 3
    SDL_Surface* subject3Surface = IMG_Load("piece4.png");
    if (!subject3Surface) {
        printf("Failed to load piece4.png: %s\n", IMG_GetError());
        return 1;
    }
    SDL_Texture* subject3Texture = SDL_CreateTextureFromSurface(renderer, subject3Surface);
    SDL_FreeSurface(subject3Surface);

    // This is the important part -- without it, alpha won't blend
    SDL_SetTextureBlendMode(fgTexture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(subject2Texture, SDL_BLENDMODE_BLEND);
    SDL_SetTextureBlendMode(subject3Texture, SDL_BLENDMODE_BLEND);

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Background first, full window
        SDL_RenderCopy(renderer, bgTexture, NULL, NULL);

        // First subject on top because it draws on top
        SDL_RenderCopy(renderer, fgTexture, NULL, NULL);

        // Second subject on top
        SDL_RenderCopy(renderer, subject2Texture, NULL, NULL);

        // Third subject on top
        SDL_RenderCopy(renderer, subject3Texture, NULL, NULL);

        SDL_RenderPresent(renderer); // vsync paces this
    }

    SDL_DestroyTexture(fgTexture);
    SDL_DestroyTexture(bgTexture);
    SDL_DestroyTexture(subject2Texture);
    SDL_DestroyTexture(subject3Texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}