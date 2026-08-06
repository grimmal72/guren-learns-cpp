// Shinobi-style intro effect: hold on the detailed sprite, then alpha-fade
// into a flat red silhouette. Both PNGs share the same alpha shape, so the
// crossfade only ever affects pixels that were already opaque-ish -- the
// transparent background never picks up any red tint.
//

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <algorithm>

const int WINDOW_W = 640;
const int WINDOW_H = 480;
const int SPRITE_W = 300;
const int SPRITE_H = 300;

const Uint32 HOLD_MS = 2000; // show the plain sprite for 3 seconds
const Uint32 FADE_MS = 1200; // then fade to red over 1.5 seconds

enum class Phase { Hold, Fading, Done };

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        std::cerr << "Failed to load " << path << ": " << IMG_GetError() << "\n";
        return nullptr;
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    return texture;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init failed: " << IMG_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Shinobi Fade Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "CreateWindow failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        std::cerr << "CreateRenderer failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Texture* ninjaTexture = LoadTexture(renderer, "ninja1.png"); // detailed sprite
    SDL_Texture* redTexture = LoadTexture(renderer, "ninja2.png"); // red silhouette, same alpha shape
    if (!ninjaTexture || !redTexture) {
        SDL_Quit();
        return 1;
    }

    SDL_Rect destRect{
        (WINDOW_W - SPRITE_W) / 2,
        (WINDOW_H - SPRITE_H) / 2,
        SPRITE_W, SPRITE_H
    };

    Phase phase = Phase::Hold;
    Uint32 phaseStart = SDL_GetTicks();
    bool running = true;

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;
            // Press R to replay the effect from the start.
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                phase = Phase::Hold;
                phaseStart = SDL_GetTicks();
            }
        }

        Uint32 now = SDL_GetTicks();
        Uint32 elapsed = now - phaseStart;

        if (phase == Phase::Hold && elapsed >= HOLD_MS) {
            phase = Phase::Fading;
            phaseStart = now;
            elapsed = 0;
        }
        else if (phase == Phase::Fading && elapsed >= FADE_MS) {
            phase = Phase::Done;
        }

        // --- draw ---
        SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
        SDL_RenderClear(renderer);

        // Base sprite is always drawn at full opacity.
        SDL_SetTextureAlphaMod(ninjaTexture, 255);
        SDL_RenderCopy(renderer, ninjaTexture, nullptr, &destRect);

        if (phase == Phase::Fading) {
            float t = std::min(1.0f, elapsed / static_cast<float>(FADE_MS));
            Uint8 a = static_cast<Uint8>(t * 255.0f);
            SDL_SetTextureAlphaMod(redTexture, a);
            SDL_RenderCopy(renderer, redTexture, nullptr, &destRect);
        }
        else if (phase == Phase::Done) {
            SDL_SetTextureAlphaMod(redTexture, 255);
            SDL_RenderCopy(renderer, redTexture, nullptr, &destRect);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 fps
    }

    SDL_DestroyTexture(ninjaTexture);
    SDL_DestroyTexture(redTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}