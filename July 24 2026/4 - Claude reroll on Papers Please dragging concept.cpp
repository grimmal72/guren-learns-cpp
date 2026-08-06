#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>

const char* FONT_PATH = "arial.ttf";   // <-- point at a real .ttf on your system
const char* IMAGE_PATH = "20x20.png";    // <-- your 20x20 image, PNG with alpha is fine
const int WINDOW_W = 800;
const int WINDOW_H = 600;
const int ITEM_W = 20;
const int ITEM_H = 20;

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
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init failed: " << IMG_GetError() << "\n";
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Clip Rect + Drag Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );


    TTF_Font* font = TTF_OpenFont(FONT_PATH, 72);
    if (!font) {
        std::cerr << "TTF_OpenFont failed: " << TTF_GetError() << "\n";
        return 1;
    }
    SDL_Color black = { 0, 0, 0, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, "PASSPORT", black);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    int textW = textSurface->w;
    int textH = textSurface->h;
    SDL_FreeSurface(textSurface);

    // --- Draggable item ---
    SDL_Texture* itemTexture = IMG_LoadTexture(renderer, IMAGE_PATH);
    if (!itemTexture) {
        std::cerr << "IMG_LoadTexture failed: " << IMG_GetError() << "\n";
        std::cerr << "(Point IMAGE_PATH at a real 20x20 image file)\n";
        return 1;
    }

    // Start centered in the window, so it's fully visible at launch.
    SDL_Rect itemRect = {
        (WINDOW_W - ITEM_W) / 2,
        (WINDOW_H - ITEM_H) / 2,
        ITEM_W,
        ITEM_H
    };

    bool dragging = false;
    int dragOffsetX = 0; // mouse position relative to itemRect.x/y at pickup
    int dragOffsetY = 0;

    // The "desk" / panel rect.
    SDL_Rect box = { 250, 200, 300, 200 };

    SDL_Rect textDest = { box.x - 60, box.y - 40, textW, textH };

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                running = false;

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX = event.button.x;
                int mouseY = event.button.y;
                SDL_Point mousePoint = { mouseX, mouseY };
                if (SDL_PointInRect(&mousePoint, &itemRect)) {
                    dragging = true;
                    dragOffsetX = mouseX - itemRect.x;
                    dragOffsetY = mouseY - itemRect.y;
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                dragging = false;
            }

            if (event.type == SDL_MOUSEMOTION && dragging) {
                itemRect.x = event.motion.x - dragOffsetX;
                itemRect.y = event.motion.y - dragOffsetY;
            }
        }

        // 1. Clear to white.
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // 2. Draw the red box (unclipped, so the box itself is always whole).
        SDL_SetRenderDrawColor(renderer, 200, 30, 30, 255);
        SDL_RenderFillRect(renderer, &box);

        // 3. Clip to the box, draw everything that should crop at its edges.
        SDL_RenderSetClipRect(renderer, &box);
        SDL_RenderCopy(renderer, textTexture, nullptr, &textDest);
        SDL_RenderCopy(renderer, itemTexture, nullptr, &itemRect);
        SDL_RenderSetClipRect(renderer, nullptr);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(textTexture);
    SDL_DestroyTexture(itemTexture);
    TTF_CloseFont(font);
    TTF_Quit();
    IMG_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}