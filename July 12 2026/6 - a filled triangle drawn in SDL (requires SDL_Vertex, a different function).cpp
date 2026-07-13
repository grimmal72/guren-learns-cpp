#include <SDL.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    const int SCREEN_HEIGHT = 480;
    const int SCREEN_WIDTH = 640;

    SDL_Window* window = SDL_CreateWindow("Filled Triangle Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    SDL_Color red = { 255, 0, 0, 255 };

    SDL_Vertex vertices[3] = {
        { { SCREEN_WIDTH / 2.0f, 100.0f },        red, { 0, 0 } }, // top
        { { 150.0f, 380.0f },                     red, { 0, 0 } }, // bottom-left
        { { SCREEN_WIDTH - 150.0f, 380.0f },       red, { 0, 0 } }  // bottom-right
    };

    // for not using a texture in the triangle, texture=nullptr, indices=nullptr (just use vertices in order), num_indices=0
    SDL_RenderGeometry(renderer, nullptr, vertices, 3, nullptr, 0);

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