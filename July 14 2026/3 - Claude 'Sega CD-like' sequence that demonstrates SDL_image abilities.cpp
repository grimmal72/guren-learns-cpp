// sega_cd_attract.cpp
//
// Sega CD BIOS attract-screen style demo: drastic scale pulsing,
// Lissajous drift, slow rotation, rainbow tint, and a trailing
// smear effect from a low-alpha black overlay instead of a full clear.
//
// Build (MSYS2 UCRT64):
//   g++ sega_cd_attract.cpp -o attract.exe -lSDL2main -lSDL2 -lSDL2_image
//
// Put a 640x480 PNG named "image.png" next to the executable, or
// change IMAGE_PATH below.

#include <SDL.h>
#include <SDL_image.h>
#include <cmath>

const int SCREEN_W = 1600;
const int SCREEN_H = 900;
const char* IMAGE_PATH = "image.png";

void hsv_to_rgb(float h, float s, float v, Uint8* r, Uint8* g, Uint8* b) {
    float c = v * s;
    float x = c * (1.0f - std::fabs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;
    float rf = 0, gf = 0, bf = 0;

    if (h < 60) { rf = c; gf = x; bf = 0; }
    else if (h < 120) { rf = x; gf = c; bf = 0; }
    else if (h < 180) { rf = 0; gf = c; bf = x; }
    else if (h < 240) { rf = 0; gf = x; bf = c; }
    else if (h < 300) { rf = x; gf = 0; bf = c; }
    else { rf = c; gf = 0; bf = x; }

    *r = static_cast<Uint8>((rf + m) * 255);
    *g = static_cast<Uint8>((gf + m) * 255);
    *b = static_cast<Uint8>((bf + m) * 255);
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        SDL_Log("IMG_Init failed: %s", IMG_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Attract",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_FULLSCREEN
    );
    if (!window) {
        SDL_Log("CreateWindow failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        SDL_Log("CreateRenderer failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Texture* texture = IMG_LoadTexture(renderer, IMAGE_PATH);
    if (!texture) {
        SDL_Log("IMG_LoadTexture failed: %s", IMG_GetError());
        return 1;
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    const int IMG_W = 640;
    const int IMG_H = 480;

    bool running = true;
    Uint64 start_ticks = SDL_GetTicks64();

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = false;
        }

        double t = (SDL_GetTicks64() - start_ticks) / 1000.0;

        // Drastic scale pulse: swings from tiny to huge and back.
        float scale = 2.3f + 2.0f * static_cast<float>(std::sin(t * 1.3));
        if (scale < 0.25f) scale = 0.25f;

        // Slow Lissajous drift around the middle of the screen.
        float margin_x = SCREEN_W * 0.28f;
        float margin_y = SCREEN_H * 0.22f;
        float cx = SCREEN_W / 2.0f + margin_x * static_cast<float>(std::sin(t * 0.45));
        float cy = SCREEN_H / 2.0f + margin_y * static_cast<float>(std::sin(t * 0.63 + 1.1));

        // Gentle continuous rotation.
        double angle = t * 12.0;

        // Rainbow color cycle.
        float hue = std::fmod(t * 60.0, 360.0);
        Uint8 r, g, b;
        hsv_to_rgb(hue, 0.55f, 1.0f, &r, &g, &b);
        SDL_SetTextureColorMod(texture, r, g, b);

        // Trail: paint a low-alpha black rect instead of a hard clear
        // so previous frames smear rather than vanish instantly.
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect full_screen = { 0, 0, SCREEN_W, SCREEN_H };
        SDL_RenderFillRect(renderer, &full_screen);

        int draw_w = static_cast<int>(IMG_W * scale);
        int draw_h = static_cast<int>(IMG_H * scale);
        SDL_Rect dest;
        dest.w = draw_w;
        dest.h = draw_h;
        dest.x = static_cast<int>(cx - draw_w / 2.0f);
        dest.y = static_cast<int>(cy - draw_h / 2.0f);

        SDL_RenderCopyEx(renderer, texture, nullptr, &dest, angle, nullptr, SDL_FLIP_NONE);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}