#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <iostream>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 800;

// Builds a texture containing one blurred rounded rect, sized to include padding.
// The rect is drawn starting at (padding, padding) inside the texture, so the
// blur layers (which extend outward by up to `layers` px) don't get clipped.
SDL_Texture* createBlurredRoundedRect(SDL_Renderer* renderer,
    int rectW, int rectH, int radius,
    Uint8 r, Uint8 g, Uint8 b, Uint8 a,
    int layers, int padding,
    int& outTexW, int& outTexH)
{
    outTexW = rectW + padding * 2;
    outTexH = rectH + padding * 2;

    SDL_Texture* texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        outTexW, outTexH
    );
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); // critical: alpha survives compositing

    SDL_Texture* prevTarget = SDL_GetRenderTarget(renderer);
    SDL_SetRenderTarget(renderer, texture);

    // Transparent clear
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    int x1 = padding, y1 = padding;
    int x2 = padding + rectW, y2 = padding + rectH;

    for (int i = 0; i < layers; i++) {
        roundedBoxRGBA(renderer, x1 - i, y1 - i, x2 + i, y2 + i, radius,
            r, g, b, a - (i * 20));
    }

    SDL_SetRenderTarget(renderer, prevTarget);
    return texture;
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Rothko Painting", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // --- Build the top rectangle as a texture, once, before the loop ---
    int topRectW = 19 * (WINDOW_WIDTH / 20) - (WINDOW_WIDTH / 20); // x2 - x1
    int topRectH = 250 - 50;                                       // y2 - y1
    int topRectX1 = WINDOW_WIDTH / 20;
    int topRectY1 = 50;
    int padding = 16; // >= number of blur layers, gives the glow room to breathe

    int textureW, textureH;
    SDL_Texture* topTexture = createBlurredRoundedRect(
        renderer, topRectW, topRectH, 30,
        238, 230, 113, 255,
        12, padding, textureW, textureH
    );

    float topAngle = -1.8f; // your "human error" tilt, in degrees

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) running = false;
        }

        SDL_SetRenderDrawColor(renderer, 87, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Destination rect: same position/size as the *padded* texture,
        // anchored so the rect lands where topRectX1/Y1 originally would.
        SDL_Rect destRect;
        destRect.x = topRectX1 - padding;
        destRect.y = topRectY1 - padding;
        destRect.w = textureW;
        destRect.h = textureH;

        SDL_RenderCopyEx(renderer, topTexture, nullptr, &destRect,
            topAngle, nullptr, SDL_FLIP_NONE);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(topTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}