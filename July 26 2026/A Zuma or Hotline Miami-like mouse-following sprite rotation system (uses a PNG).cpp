// Mouse-facing sprite demo (Hotline Miami / Zuma style pointer rotation)
//
// Loads a real 50x50 PNG (pointer.png, alpha-blended) via SDL_image and
// rotates it every frame to face the mouse cursor. The rotation is never
// incremented — each frame it's recomputed fresh from the sprite's center
// to the current absolute mouse position, so it snaps to match the mouse
// exactly the way the crosshair/eyes do in Hotline Miami or Zuma.
//
// pointer.png must sit next to the executable (or update the path below).
// The art is drawn facing "up" by default (90 degrees on the unit circle),
// which is why a +90 degree correction is applied after atan2.
//
// Build (Linux/macOS):
//   g++ main2.cpp -o pointer_demo $(sdl2-config --cflags --libs) -lSDL2_image
//
// Windows/Visual Studio:
//   Link SDL2.lib and SDL2_image.lib. Make sure SDL2.dll, SDL2_image.dll,
//   and their PNG-decoding dependencies (libpng, zlib) sit next to the .exe.

#include <SDL.h>
#include <SDL_image.h>
#include <cmath>
#include <cstdio>

constexpr double PI = 3.14159265358979323846;
constexpr int SPRITE_W = 50;
constexpr int SPRITE_H = 50;
constexpr int WINDOW_W = 800;
constexpr int WINDOW_H = 600;
constexpr const char* SPRITE_PATH = "pointer.png";

SDL_Texture* loadPngTexture(SDL_Renderer* renderer, const char* path) {
    SDL_Texture* texture = IMG_LoadTexture(renderer, path);
    if (!texture) {
        std::printf("IMG_LoadTexture failed for '%s': %s\n", path, IMG_GetError());
        return nullptr;
    }
    // Ensure alpha channel is respected when compositing over the
    // background each frame (this is the "blended" part).
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    return texture;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::printf("IMG_Init failed: %s\n", IMG_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Mouse-Facing PNG Sprite Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::printf("SDL_CreateWindow failed: %s\n", SDL_GetError());
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        std::printf("SDL_CreateRenderer failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Texture* spriteTexture = loadPngTexture(renderer, SPRITE_PATH);
    if (!spriteTexture) {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Rect destRect;
    destRect.w = SPRITE_W;
    destRect.h = SPRITE_H;
    destRect.x = WINDOW_W / 2 - SPRITE_W / 2;
    destRect.y = WINDOW_H / 2 - SPRITE_H / 2;

    double angleDegree = 0.0;
    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN &&
                event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        // --- Recompute facing angle from sprite center to mouse, every frame ---
        int mouseX, mouseY;
        SDL_GetMouseState(&mouseX, &mouseY);

        double centerX = destRect.x + destRect.w / 2.0;
        double centerY = destRect.y + destRect.h / 2.0;

        double dx = mouseX - centerX;
        double dy = mouseY - centerY;

        // atan2 in screen space (y grows downward) already matches the
        // clockwise rotation direction SDL_RenderCopyEx expects.
        double angleRadian = std::atan2(dy, dx);
        double rawDegree = angleRadian * (180.0 / PI);

        // +90 corrects for pointer.png's default "facing up" pose.
        // Drop this offset if your art is drawn facing right by default.
        angleDegree = rawDegree + 90.0;

        // --- Render ---
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopyEx(
            renderer,
            spriteTexture,
            nullptr,       // srcRect: use whole texture
            &destRect,     // destRect: position + size on screen
            angleDegree,      // rotation in degrees, clockwise
            nullptr,       // pivot: nullptr = rotate around dstRect's center
            SDL_FLIP_NONE
        );

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(spriteTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}