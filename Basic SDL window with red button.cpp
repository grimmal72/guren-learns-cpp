// This is a good template for any kind of C++ program that just needs a button that does something. It's very simple.
// This could be used for a menu screen, an idle game where you click and the number goes up, a music soundboard, lots of things.



#include <SDL.h>
#include <iostream>

// Window settings
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize.\n";
        return 1;
    }

    // Create centered window
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Button Example",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window)
    {
        std::cout << "Window creation failed.\n";
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!renderer)
    {
        std::cout << "Renderer creation failed.\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    // One button centered in the window
    SDL_Rect button = {
        (SCREEN_WIDTH - 200) / 2,   // x
        (SCREEN_HEIGHT - 100) / 2,  // y
        200,                        // width
        100                         // height
    };

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_Point mousePoint =
                {
                    event.button.x,
                    event.button.y
                };

                if (SDL_PointInRect(&mousePoint, &button))
                {
                    // Button was clicked.
                    // (Nothing happens yet.)
                }
            }
        }

        // White background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Red button
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &button);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
