// press_start.cpp
// Minimal blinking "PRESS START" title screen demo.
// Blinks on/off every 500ms, like classic arcade title screens.
//
// Compile (MSYS2 UCRT64):
//   g++ press_start.cpp -o press_start.exe -lSDL2 -lSDL2main -lSDL2_ttf
//
// You need a .ttf font file — point FONT_PATH at one you have.

#include <SDL.h>
#include <SDL_ttf.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const Uint32 BLINK_INTERVAL_MS = 500;
const char* FONT_PATH = "(some ttf fonts/arial.ttf"; // change this to a font you actually have
const int FONT_SIZE = 32;
const char* MESSAGE = "PRESS START";

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    if (TTF_Init() != 0) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Press Start Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        SDL_Log("CreateWindow failed: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        SDL_Log("CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!font) {
        SDL_Log("TTF_OpenFont failed: %s (did you set FONT_PATH?)", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Render the text to a texture once — no need to re-render every frame,
    // we just choose whether to draw it or not.
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, MESSAGE, white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textWidth = textSurface->w;
    int textHeight = textSurface->h;
    SDL_FreeSurface(textSurface);

    SDL_Rect textRect;
    textRect.x = (WINDOW_WIDTH - textWidth) / 2;
    textRect.y = (WINDOW_HEIGHT - textHeight) / 2;
    textRect.w = textWidth;
    textRect.h = textHeight;

    bool running = true;
    bool textVisible = true;
    Uint32 lastToggleTime = SDL_GetTicks();

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                running = false;
            }
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastToggleTime >= BLINK_INTERVAL_MS) {
            textVisible = !textVisible;
            lastToggleTime = currentTime;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (textVisible) {
            SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
