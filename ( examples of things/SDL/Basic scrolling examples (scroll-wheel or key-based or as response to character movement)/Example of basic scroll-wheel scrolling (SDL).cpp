#include <SDL.h>
#include <SDL_image.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    const int SCREEN_WIDTH = 640;
    const int SCREEN_HEIGHT = 480;

    SDL_Window* window = SDL_CreateWindow("Scroll Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface* surface = IMG_Load("scroll-background.png"); // expects a 640x1000 PNG
    SDL_Texture* image = SDL_CreateTextureFromSurface(renderer, surface);

    int imageWidth, imageHeight;
    SDL_QueryTexture(image, nullptr, nullptr, &imageWidth, &imageHeight);
    SDL_FreeSurface(surface); // pixel data is on the GPU now, surface no longer needed

    const int MAX_SCROLL = imageHeight - SCREEN_HEIGHT;
    int scrollY = 0;

    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_MOUSEWHEEL) {
                scrollY -= event.wheel.y * 20;

                if (scrollY < 0) scrollY = 0;
                if (scrollY > MAX_SCROLL) scrollY = MAX_SCROLL;
            }
        }

        SDL_Rect srcRect = { 0, scrollY, SCREEN_WIDTH, SCREEN_HEIGHT };
        SDL_RenderCopy(renderer, image, &srcRect, nullptr);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}
