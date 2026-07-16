#include <SDL.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    const int SCREEN_HEIGHT = 480;
    const int SCREEN_WIDTH = 640;
    const float SCALE = 20.0f;

    SDL_Window* window = SDL_CreateWindow("X Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetScale(renderer, SCALE, SCALE);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    // Logical space is 640/20 = 32 units wide, 480/20 = 24 units tall.
    // Center is (16, 12). Build an X radiating out from there.
    SDL_Point xShape[] = {
        { 16, 12 },   // center
        { 14, 10 }, { 15, 11 }, { 17, 13 }, { 18, 14 }, // top-left to bottom-right arm
        { 18, 10 }, { 17, 11 }, { 15, 13 }, { 14, 14 }  // top-right to bottom-left arm
    };

    SDL_RenderDrawPoints(renderer, xShape, 9);

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