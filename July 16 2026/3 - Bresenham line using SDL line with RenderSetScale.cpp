#include <SDL.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    const int SCREEN_HEIGHT = 480;
    const int SCREEN_WIDTH = 640;
    const float SCALE = 20.0f;

    SDL_Window* window = SDL_CreateWindow("Steep Line Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetScale(renderer, SCALE, SCALE);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Rise of 15 over a run of 1, in logical units.
    // atan(15/1) ≈ 86.2 degrees from horizontal — close to your 87 target,
    // and integer coordinates make the stepping land on clean pixel boundaries.
    SDL_RenderDrawLine(renderer, 5, 2, 6, 17);

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