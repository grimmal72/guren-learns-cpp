// clip_demo.cpp
//
// Demonstrates SDL_RenderSetClipRect: a red box sits in the window,
// and big black SDL_ttf text is drawn overflowing its edges on
// purpose -- SDL crops (clips) anything outside the box automatically.
//
// This is the same mechanism you'd use for a Papers-Please-style desk
// or a "messy inventory" panel: draw items wherever they logically are,
// set the clip rect to the panel/box, and let SDL handle the cropping.
//
// Build (vcpkg-style, adjust to your project setup):
//   cl clip_demo.cpp /I<sdl2_include> /I<sdl2_ttf_include> /link SDL2.lib SDL2main.lib SDL2_ttf.lib
//
// You'll need a .ttf font file. Change FONT_PATH below to point at one
// (any TTF on your system works, e.g. C:\Windows\Fonts\arial.ttf on Windows).

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

const char* FONT_PATH = "arial.ttf"; // <-- change this to a real font path
const int WINDOW_W = 800;
const int WINDOW_H = 600;

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }
    if (TTF_Init() < 0) {
        std::cerr << "TTF_Init failed: " << TTF_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Clip Rect Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    // Big font so the text is guaranteed to overflow a modest box.
    TTF_Font* font = TTF_OpenFont(FONT_PATH, 72);
    if (!font) {
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << "\n";
        std::cerr << "(Point FONT_PATH at a real .ttf file on your system)\n";
        return 1;
    }

    // Render some text to a texture once, up front.
    SDL_Color black = { 0, 0, 0, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, "PASSPORT", black);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textW = textSurface->w;
    int textH = textSurface->h;
    SDL_FreeSurface(textSurface);

    // The "desk" / panel rect -- smaller than the window, sitting inside it.
    SDL_Rect box = { 250, 200, 251, 201 }; // x, y, w, h

    // Position the text so it deliberately overflows all four edges
    // of the box. Negative offset = starts above/left of the box.
    SDL_Rect textDest = {
        box.x - 60,               // sticks out the left
        box.y - 40,               // sticks out the top
        textW,
        textH
    };
    // (With a 72pt "PASSPORT" and a 300x200 box, it'll also overflow
    //  the right and bottom edges naturally -- feel free to nudge the
    //  numbers to see it clipped from any side.)

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                running = false;

            // Optional: nudge the text around with arrow keys to feel
            // out how the cropping tracks the box edges live.
            if (event.type == SDL_KEYDOWN) {
                const int step = 10;
                if (event.key.keysym.sym == SDLK_LEFT)  textDest.x -= step;
                if (event.key.keysym.sym == SDLK_RIGHT) textDest.x += step;
                if (event.key.keysym.sym == SDLK_UP)    textDest.y -= step;
                if (event.key.keysym.sym == SDLK_DOWN)  textDest.y += step;
            }
        }

        // 1. Clear to white.
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // 2. Draw the red box itself. No clip rect active yet, so this
        //    draws normally (full box visible).
        SDL_SetRenderDrawColor(renderer, 200, 30, 30, 255);
        SDL_RenderFillRect(renderer, &box);

        // 3. THE KEY STEP: restrict the renderer to only draw inside `box`.
        SDL_RenderSetClipRect(renderer, &box);

        // 4. Draw the text at its overflowing position. Even though
        //    textDest extends outside `box`, only the portion inside
        //    the clip rect actually shows up on screen.
        SDL_RenderCopy(renderer, textTexture, nullptr, &textDest);

        // 5. IMPORTANT: clear the clip rect afterward, or every future
        //    draw call (menus, HUD, next frame's box outline, etc.)
        //    stays clipped to that same region.
        SDL_RenderSetClipRect(renderer, nullptr);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}