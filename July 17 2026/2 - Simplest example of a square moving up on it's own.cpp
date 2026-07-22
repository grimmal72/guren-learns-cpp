#include <SDL.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    const int SCREEN_HEIGHT = 480;
    const int SCREEN_WIDTH = 640;

    SDL_Window* window = SDL_CreateWindow("Move Right Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Rect box = { 0, 220, 40, 40 };

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }

        // This animates in the up direction. To make it go down, change
        // -= to +=. Also, making it go right or left would be extremely
        // easy. Just change y to x and play around with -= and +=.
        box.y -= 2;

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &box);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // roughly 60fps pacing
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}