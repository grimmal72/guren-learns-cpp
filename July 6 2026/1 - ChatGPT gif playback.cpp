#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <cstdio>

const int WIDTH = 220;
const int HEIGHT = 220;
const int FRAME_COUNT = 8;
const int FRAME_TIME = 100; // milliseconds per frame (10 FPS)

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        std::cout << IMG_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "PNG Animation",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    SDL_Texture* frames[FRAME_COUNT];

    // Load every frame
    for (int i = 0; i < FRAME_COUNT; i++)
    {
        char filename[64];
        sprintf_s(filename, sizeof(filename), "frame_%03d.png", i + 1);

        SDL_Surface* surface = IMG_Load(filename);

        if (!surface)
        {
            std::cout << "Couldn't load " << filename << std::endl;
            return 1;
        }

        frames[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        if (!frames[i])
        {
            std::cout << SDL_GetError() << std::endl;
            return 1;
        }
    }

    SDL_Rect destination = { 0, 0, WIDTH, HEIGHT };

    bool running = true;
    SDL_Event event;

    Uint32 lastFrameTime = SDL_GetTicks();
    int currentFrame = 0;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        Uint32 now = SDL_GetTicks();

        if (now - lastFrameTime >= FRAME_TIME)
        {
            currentFrame++;
            currentFrame %= FRAME_COUNT;
            lastFrameTime = now;
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(
            renderer,
            frames[currentFrame],
            NULL,
            &destination
        );

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    for (int i = 0; i < FRAME_COUNT; i++)
        SDL_DestroyTexture(frames[i]);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}