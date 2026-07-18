#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    SDL_Window* window = SDL_CreateWindow("Horizontal Scroll Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Surface* surface = IMG_Load("scroll-background-wide.png"); // expects a 1200x480 PNG
    SDL_Texture* image = SDL_CreateTextureFromSurface(renderer, surface);

    int imageWidth, imageHeight;
    SDL_QueryTexture(image, nullptr, nullptr, &imageWidth, &imageHeight);
    SDL_FreeSurface(surface);

    const int MAX_SCROLL = imageWidth - SCREEN_WIDTH; // 560
    int scrollX = 0;
    const int SCROLL_SPEED = 5; // pixels per frame while a key is held

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;
        }

        // Continuous held-key check, not a single keydown event —
        // this gives smooth movement for as long as the key stays down.
        const Uint8* keys = SDL_GetKeyboardState(nullptr);
        if (keys[SDL_SCANCODE_LEFT])  scrollX -= SCROLL_SPEED;
        if (keys[SDL_SCANCODE_RIGHT]) scrollX += SCROLL_SPEED;

        if (scrollX < 0) scrollX = 0;
        if (scrollX > MAX_SCROLL) scrollX = MAX_SCROLL;

        SDL_Rect srcRect = { scrollX, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, image, &srcRect, nullptr);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // roughly 60fps pacing
    }

    SDL_DestroyTexture(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}