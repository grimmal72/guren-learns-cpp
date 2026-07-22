// rounded_rect_example.cpp
//
// Minimal SDL2 + SDL2_gfx example showing a rounded-corner rectangle,
// drawn via roundedBoxRGBA (the SDL2_gfx equivalent of CSS border-radius).
//
// Build (MSYS2/UCRT64):
//   g++ rounded_rect_example.cpp -o rounded_rect_example -lSDL2 -lSDL2_gfx -lmingw32 -mwindows
//
// Build (Linux, apt install libsdl2-dev libsdl2-gfx-dev):
//   g++ rounded_rect_example.cpp -o rounded_rect_example -lSDL2 -lSDL2_gfx
//
// Build (Visual Studio):
//   Link against SDL2.lib, SDL2main.lib, SDL2_gfx.lib and make sure the
//   corresponding DLLs are next to the .exe.

#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Rounded Rect Example",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
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
        SDL_SetRenderDrawColor(renderer, 24, 24, 32, 255);
        SDL_RenderClear(renderer);

        // Rounded rect: x1, y1, x2, y2, radius, r, g, b, a
        // (x1,y1) top-left, (x2,y2) bottom-right — note this is corner
        // coordinates, not x/y/w/h like SDL_Rect.
        int x1 = 170, y1 = 140;
        int x2 = 470, y2 = 340;
        int radius = 30;

        roundedBoxRGBA(renderer, x1, y1, x2, y2, radius,
            70, 130, 220, 255);

        // Outline on top, slightly darker, so the rounded edge reads clearly
        roundedRectangleRGBA(renderer, x1, y1, x2, y2, radius,
            20, 40, 90, 255);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}