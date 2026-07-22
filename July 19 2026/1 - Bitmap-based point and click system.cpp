#include <SDL.h>
#include <iostream>

// // IntelliSense and the Visual Studio compiler dislike this simpler version of the constants. I've included the replacement 5 lines uncommented, but I do prefer this version.
// const int SCREEN_WIDTH = 640;
// const int SCREEN_HEIGHT = 480;
// const float SCALE = 20.0f;
// const int LOGICAL_WIDTH = SCREEN_WIDTH / (int)SCALE;
// const int LOGICAL_HEIGHT = SCREEN_HEIGHT / (int)SCALE;

constexpr int SCREEN_WIDTH = 640;
constexpr int SCREEN_HEIGHT = 480;
constexpr float SCALE = 20.0f;

constexpr int LOGICAL_WIDTH = SCREEN_WIDTH / static_cast<int>(SCALE);
constexpr int LOGICAL_HEIGHT = SCREEN_HEIGHT / static_cast<int>(SCALE);

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cout << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Clickable X",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED);

    SDL_RenderSetScale(renderer, SCALE, SCALE);

    //---------------------------------------------------
    // Define the X
    //---------------------------------------------------

    SDL_Point xShape[] =
    {
        {16,12},                     // center

        {14,10},
        {15,11},
        {17,13},
        {18,14},

        {18,10},
        {17,11},
        {15,13},
        {14,14}
    };

    const int X_ID = 1;

    //---------------------------------------------------
    // Hotspot mask
    //---------------------------------------------------

    int hotspot[LOGICAL_HEIGHT][LOGICAL_WIDTH] = {};

    for (const SDL_Point& p : xShape)
    {
        hotspot[p.y][p.x] = X_ID;
    }

    //---------------------------------------------------

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;

            if (event.type == SDL_MOUSEBUTTONDOWN &&
                event.button.button == SDL_BUTTON_LEFT)
            {
                // Convert from window pixels back into logical coordinates.
                int mx = event.button.x / (int)SCALE;
                int my = event.button.y / (int)SCALE;

                if (mx >= 0 &&
                    mx < LOGICAL_WIDTH &&
                    my >= 0 &&
                    my < LOGICAL_HEIGHT)
                {
                    if (hotspot[my][mx] == X_ID)
                    {
                        std::cout << "You clicked the X!" << std::endl;
                    }
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawPoints(renderer, xShape, SDL_arraysize(xShape));

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}