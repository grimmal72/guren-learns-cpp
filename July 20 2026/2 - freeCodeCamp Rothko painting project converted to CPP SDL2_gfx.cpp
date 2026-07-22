#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 800;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Rothko Painting CSS Project Translation to CPP SDL",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cout << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        std::cout << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // roundedBoxRGBA needs alpha blending enabled to draw semi-transparent
    // edges cleanly; harmless to leave on even for opaque fills.
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        // Background
        SDL_SetRenderDrawColor(renderer, 87, 0, 0, 255);
        SDL_RenderClear(renderer);


        // THE NEXT 10 OR SO LINES ARE FOR THE TOP RECTANGLE
        int topRectX1 = WINDOW_WIDTH / 20, topRectY1 = 50;
        int topRectX2 = 19 * (WINDOW_WIDTH / 20), topRectY2 = 250;
        int topRectRadius = 30;

        // roundedBoxRGBA(renderer, topRectX1, topRectY1, topRectX2, topRectY2, topRectRadius,
        //     238, 230, 113, 255);

        for (int i = 0; i < 12; i++) {
            roundedBoxRGBA(renderer, topRectX1 - i, topRectY1 - i, topRectX2 + i, topRectY2 + i, topRectRadius,
                238, 230, 113, 255 - (i * 20));
        }


        // THE NEXT 10 OR SO LINES ARE FOR THE MIDDLE RECTANGLE
        int middleRectX1 = WINDOW_WIDTH / 30, middleRectY1 = 300;
        int middleRectX2 = 29 * (WINDOW_WIDTH / 30), middleRectY2 = 500;
        int middleRectRadius = 30;

        // roundedBoxRGBA(renderer, middleRectX1, middleRectY1, middleRectX2, middleRectY2, middleRectRadius,
        //     120, 0, 0, 255);

        for (int i = 0; i < 12; i++) {
            roundedBoxRGBA(renderer, middleRectX1 - i, middleRectY1 - i, middleRectX2 + i, middleRectY2 + i, middleRectRadius,
                120, 0, 0, 255 - (i * 20));
        }

        // THE NEXT 10 OR SO LINES ARE FOR THE BOTTOM RECTANGLE
        int bottomRectX1 = WINDOW_WIDTH / 25, bottomRectY1 = 550;
        int bottomRectX2 = 24 * (WINDOW_WIDTH / 25), bottomRectY2 = 750;
        int bottomRectRadius = 30;

        // roundedBoxRGBA(renderer, bottomRectX1, bottomRectY1, bottomRectX2, bottomRectY2, bottomRectRadius,
        //     155, 0, 0, 255);

        for (int i = 0; i < 12; i++) {
            roundedBoxRGBA(renderer, bottomRectX1 - i, bottomRectY1 - i, bottomRectX2 + i, bottomRectY2 + i, bottomRectRadius,
                155, 0, 0, 255 - (i * 20));
        }


        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}