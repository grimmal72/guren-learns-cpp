// main.cpp
#include <SDL.h>
#include <cmath>
#include <iostream>

constexpr double PI = 3.14159265358979323846;
constexpr int SPRITE_SIZE = 50;
constexpr int WINDOW_W = 800;
constexpr int WINDOW_H = 600;

// Builds a 50x50 texture with an arrow pointing straight UP (0,-1)
// at zero rotation. Replace this whole function with a call to
// IMG_LoadTexture(renderer, "arrow.png") if you're using SDL_image.
SDL_Texture* createArrowTexture(SDL_Renderer* renderer) {
    SDL_Texture* texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        SPRITE_SIZE, SPRITE_SIZE
    );
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 220, 60, 255);

    // Simple upward-pointing triangle/arrow, centered in the 50x50 box
    SDL_Point shaft[2] = { {25, 45}, {25, 10} };
    SDL_RenderDrawLine(renderer, shaft[0].x, shaft[0].y, shaft[1].x, shaft[1].y);
    SDL_RenderDrawLine(renderer, 25, 45, 25, 45); // no-op, keeps line thickness consistent

    SDL_Point head[4] = { {10, 20}, {25, 5}, {40, 20}, {10, 20} };
    SDL_RenderDrawLines(renderer, head, 4);

    // Thicken lines a bit by drawing offset copies
    for (int dx = -1; dx <= 1; ++dx) {
        SDL_RenderDrawLine(renderer, 25 + dx, 45, 25 + dx, 10);
        SDL_Point headOffset[4] = {
            {10 + dx, 20}, {25 + dx, 5}, {40 + dx, 20}, {10 + dx, 20}
        };
        SDL_RenderDrawLines(renderer, headOffset, 4);
    }

    SDL_SetRenderTarget(renderer, nullptr);
    return texture;
}

double computeFacingAngleDeg(SDL_Rect destRect) {
    int mouseX, mouseY;
    SDL_GetMouseState(&mouseX, &mouseY);

    double centerX = destRect.x + destRect.w / 2.0;
    double centerY = destRect.y + destRect.h / 2.0;

    double dx = mouseX - centerX;
    double dy = mouseY - centerY;

    double angleRad = std::atan2(dy, dx);
    double angleDeg = angleRad * (180.0 / PI);

    // +90 corrects for artwork drawn facing "up" by default
    return angleDeg + 90.0;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Mouse-Facing Sprite",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE
    );
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Texture* spriteTexture = createArrowTexture(renderer);

    SDL_Rect destRect;
    destRect.w = SPRITE_SIZE;
    destRect.h = SPRITE_SIZE;
    destRect.x = WINDOW_W / 2 - SPRITE_SIZE / 2;
    destRect.y = WINDOW_H / 2 - SPRITE_SIZE / 2;

    double angleDeg = 0.0; // 0 = facing up, matching the texture's default pose

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        angleDeg = computeFacingAngleDeg(destRect);

        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        SDL_RenderCopyEx(
            renderer,
            spriteTexture,
            nullptr,
            &destRect,
            angleDeg,
            nullptr,       // rotate around destRect's center
            SDL_FLIP_NONE
        );

        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60fps
    }

    SDL_DestroyTexture(spriteTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}