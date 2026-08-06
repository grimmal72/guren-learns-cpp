// drag_demo.cpp
//
// A small 20x20 image starts centered in the window. Click and hold
// on it to pick it up, move the mouse to drag it, release to drop it
// wherever it lands. The red box uses the same SDL_RenderSetClipRect
// trick from before, so dragging the item toward/past the box edges
// shows it getting cropped -- this is the core of a "messy inventory"
// interaction.
//
// Build (adjust to your setup -- needs SDL2 and SDL2_image):
//   cl drag_demo.cpp /I<sdl2_include> /I<sdl2_image_include> /link SDL2.lib SDL2main.lib SDL2_image.lib
//
// If you don't have an image handy yet, this falls back to drawing a
// solid blue 20x20 square so you can test the drag/drop/clip logic
// immediately -- swap in IMG_LoadTexture with a real PNG path once
// you've got art.

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

const int WINDOW_W = 800;
const int WINDOW_H = 600;
const int ITEM_SIZE = 20;
const char* IMAGE_PATH = "20x20.png"; // <-- point this at your 20x20 image

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow(
        "Drag Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_W, WINDOW_H,
        SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    // Try to load a real image; fall back to a solid-color texture
    // so the demo still runs with no assets at all.
    SDL_Texture* itemTexture = IMG_LoadTexture(renderer, IMAGE_PATH);
    if (!itemTexture) {
        std::cerr << "Couldn't load " << IMAGE_PATH
            << " (" << IMG_GetError() << ") -- using a placeholder square.\n";
        SDL_Surface* placeholder = SDL_CreateRGBSurfaceWithFormat(
            0, ITEM_SIZE, ITEM_SIZE, 32, SDL_PIXELFORMAT_RGBA32
        );
        SDL_FillRect(placeholder, nullptr,
            SDL_MapRGBA(placeholder->format, 60, 90, 220, 255));
        itemTexture = SDL_CreateTextureFromSurface(renderer, placeholder);
        SDL_FreeSurface(placeholder);
    }

    // Item starts centered in the window.
    SDL_Rect itemRect = {
        (WINDOW_W - ITEM_SIZE) / 2,
        (WINDOW_H - ITEM_SIZE) / 2,
        ITEM_SIZE,
        ITEM_SIZE
    };

    // The panel it can be cropped against, same idea as before.
    SDL_Rect box = { 250, 200, 300, 200 };

    bool dragging = false;
    int grabOffsetX = 0; // where inside the item you clicked, so it
    int grabOffsetY = 0; // doesn't snap to the cursor's top-left

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int mx = event.button.x;
                int my = event.button.y;
                SDL_Point mouse = { mx, my };
                if (SDL_PointInRect(&mouse, &itemRect)) {
                    dragging = true;
                    grabOffsetX = mx - itemRect.x;
                    grabOffsetY = my - itemRect.y;
                }
            }

            if (event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT) {
                dragging = false; // just stop moving it -- it stays
                // wherever it was released
            }

            if (event.type == SDL_MOUSEMOTION && dragging) {
                itemRect.x = event.motion.x - grabOffsetX;
                itemRect.y = event.motion.y - grabOffsetY;
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw the box (not clipped -- it's the container itself).
        SDL_SetRenderDrawColor(renderer, 200, 30, 30, 255);
        SDL_RenderFillRect(renderer, &box);

        // Clip anything drawn after this to the box's bounds.
        SDL_RenderSetClipRect(renderer, &box);
        SDL_RenderCopy(renderer, itemTexture, nullptr, &itemRect);
        SDL_RenderSetClipRect(renderer, nullptr);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(itemTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}