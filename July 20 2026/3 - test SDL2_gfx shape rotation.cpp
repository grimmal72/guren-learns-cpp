#include <SDL.h>
#include <SDL2_gfxPrimitives.h>

SDL_Texture* makeRoundedBoxTexture(SDL_Renderer* renderer, int w, int h, int rad,
    Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0); // fully transparent
    SDL_RenderClear(renderer);

    roundedBoxRGBA(renderer, 0, 0, w - 1, h - 1, rad, r, g, b, a);

    SDL_SetRenderTarget(renderer, NULL); // back to screen
    return texture;
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Rotate Shape Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    int w = 150, h = 100;
    SDL_Texture* shapeTexture = makeRoundedBoxTexture(renderer, w, h, 20, 80, 160, 255, 255);

    SDL_Rect destRect = { 640 / 2 - w / 2, 480 / 2 - h / 2, w, h };
    double angle = 0.0;

    int running = 1;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = 0;
        }

        angle += 1.0; // 1 degree per frame

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopyEx(renderer, shapeTexture, NULL, &destRect, angle, NULL, SDL_FLIP_NONE);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyTexture(shapeTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}