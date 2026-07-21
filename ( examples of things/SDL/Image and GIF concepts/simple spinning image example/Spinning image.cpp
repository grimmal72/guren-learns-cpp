#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Log("IMG_Init failed: %s", IMG_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Spin", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, 640, 480, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* surface = IMG_Load("image.png");
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    int w, h;
    SDL_QueryTexture(texture, NULL, NULL, &w, &h);
    SDL_Rect destRect = { 640 / 2 - w / 2, 480 / 2 - h / 2, w, h };

    double angle = 0.0;
    int running = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = 0;
        }

        angle += 1.0; // degrees per frame
        if (angle >= 360.0) angle -= 360.0;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // NULL center = rotate around the rect's own center
        SDL_RenderCopyEx(renderer, texture, NULL, &destRect, angle, NULL, SDL_FLIP_NONE);

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60fps
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}