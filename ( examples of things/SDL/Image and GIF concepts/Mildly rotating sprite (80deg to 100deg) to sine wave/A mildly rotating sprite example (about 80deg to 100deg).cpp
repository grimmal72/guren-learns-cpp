// main.cpp
// Hotline Miami-style sprite twist/wobble demo
// Requires: SDL2, SDL2_image

#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <cstdio>
#include <string>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

struct SpinningSprite {
    SDL_Texture* texture = nullptr;
    SDL_Rect     dest{};

    float amplitudeDeg = 10.0f;   // swings +/-10 deg off upright
    float frequencyHz = 0.4f;    // full cycles per second
    float phase = 0.0f;    // seconds, accumulated

    bool loadFromPNG(SDL_Renderer* renderer, const std::string& path) {
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            printf("IMG_Load failed for '%s': %s\n", path.c_str(), IMG_GetError());
            return false;
        }

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        dest.w = surface->w * 2;
        dest.h = surface->h * 2;
        SDL_FreeSurface(surface);

        if (!texture) {
            printf("SDL_CreateTextureFromSurface failed: %s\n", SDL_GetError());
            return false;
        }

        // Enable alpha blending so PNG transparency renders correctly
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        return true;
    }

    void update(float dt) {
        phase += dt;
    }

    double currentAngle() const {
        // 0 = upright in SDL's convention; oscillates -amplitude..+amplitude
        return amplitudeDeg * std::sin(phase * frequencyHz * 2.0 * M_PI);
    }

    void render(SDL_Renderer* renderer) {
        // nullptr pivot = SDL rotates around the exact center of dst,
        // giving a true central twist instead of a base-anchored wag.
        SDL_RenderCopyEx(renderer, texture, nullptr, &dest,
            currentAngle(), nullptr, SDL_FLIP_NONE);
    }

    void destroy() {
        if (texture) {
            SDL_DestroyTexture(texture);
            texture = nullptr;
        }
    }
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        printf("IMG_Init failed: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Sprite Wobble Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SpinningSprite sprite;
    // Point this at any PNG with alpha transparency.
    if (!sprite.loadFromPNG(renderer, "hotlineMiamiPhone.png")) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Center it on screen
    sprite.dest.x = (WINDOW_WIDTH - sprite.dest.w) / 2;
    sprite.dest.y = (WINDOW_HEIGHT - sprite.dest.h) / 2;

    Uint32 lastTicks = SDL_GetTicks();
    bool   running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;
        }

        Uint32 now = SDL_GetTicks();
        float  dt = (now - lastTicks) / 1000.0f;
        lastTicks = now;

        sprite.update(dt);

        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        sprite.render(renderer);

        SDL_RenderPresent(renderer);
    }

    sprite.destroy();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}
