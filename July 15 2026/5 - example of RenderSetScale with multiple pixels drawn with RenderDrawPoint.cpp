#include <SDL.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    const int SCREEN_HEIGHT = 480;
    const int SCREEN_WIDTH = 640;
    const float SCALE = 20.0f;

    SDL_Window* window = SDL_CreateWindow("Scale Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Everything drawn after this line uses SCALE-multiplied coordinates
    SDL_RenderSetScale(renderer, SCALE, SCALE);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Note: these coordinates are in LOGICAL space, not screen pixels.
    // At SCALE=20, logical (16, 12) actually lands at screen (320, 240) —
    // the center of a 640x480 window, since 640/20=32 and 480/20=24 logical units.
    SDL_RenderDrawPoint(renderer, 16, 12); // center
    SDL_RenderDrawPoint(renderer, 5, 5);   // upper-left area
    SDL_RenderDrawPoint(renderer, 26, 18); // lower-right area

    SDL_RenderPresent(renderer);

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}