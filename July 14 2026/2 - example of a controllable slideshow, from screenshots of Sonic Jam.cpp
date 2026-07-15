#include <SDL.h>
#include <SDL_image.h>
#include <cstdio>
#include <string>

const int IMAGE_COUNT = 32;
const int IMAGE_WIDTH = 640;
const int IMAGE_HEIGHT = 480;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)) {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Sonic Jam gallery slideshow proof-of-concept",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        IMAGE_WIDTH, IMAGE_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Load all 32 images up front. Files are expected to be named
    // 1.jpg through 32.jpg, sitting next to the executable (or wherever
    // the working directory is when you run it).
    SDL_Texture* textures[IMAGE_COUNT] = { nullptr };

    for (int i = 0; i < IMAGE_COUNT; i++) {
        std::string path = "( SOME REORGANIZATION/July 14 2026/" + std::to_string(i + 1) + ".jpg";
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            printf("Failed to load %s: %s\n", path.c_str(), IMG_GetError());
            continue;
        }
        textures[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
        if (!textures[i]) {
            printf("Failed to create texture for %s: %s\n", path.c_str(), SDL_GetError());
        }
    }

    int currentIndex = 0; // shows 1.jpg first
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_DOWN) {
                    currentIndex = (currentIndex + 1) % IMAGE_COUNT;
                }
                else if (event.key.keysym.sym == SDLK_UP) {
                    currentIndex = (currentIndex - 1 + IMAGE_COUNT) % IMAGE_COUNT;
                }
                else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                }
            }
        }

        SDL_RenderClear(renderer);
        if (textures[currentIndex]) {
            SDL_RenderCopy(renderer, textures[currentIndex], nullptr, nullptr);
        }
        SDL_RenderPresent(renderer);
    }

    for (int i = 0; i < IMAGE_COUNT; i++) {
        if (textures[i]) {
            SDL_DestroyTexture(textures[i]);
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}