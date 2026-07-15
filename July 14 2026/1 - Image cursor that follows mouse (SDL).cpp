#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window* window = SDL_CreateWindow(
        "Custom Cursor",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    // Load cursor PNG (with alpha) as a texture
    SDL_Surface* cursorSurface = IMG_Load("cursor.png");
    SDL_Texture* cursorTexture = SDL_CreateTextureFromSurface(renderer, cursorSurface);
    SDL_FreeSurface(cursorSurface);

    // Enable alpha blending for the texture
    SDL_SetTextureBlendMode(cursorTexture, SDL_BLENDMODE_BLEND);

    // Hide the system cursor since we're drawing our own
    SDL_ShowCursor(SDL_DISABLE);

    int mouseX = 0;
    int mouseY = 0;

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_MOUSEMOTION) {
                mouseX = event.motion.x;
                mouseY = event.motion.y;
            }
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        // Draw cursor image at mouse position (64x64)
        SDL_Rect cursorRect = { mouseX, mouseY, 64, 64 };
        SDL_RenderCopy(renderer, cursorTexture, nullptr, &cursorRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(cursorTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}