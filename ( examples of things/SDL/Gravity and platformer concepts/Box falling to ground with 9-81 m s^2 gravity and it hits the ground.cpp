#include <SDL.h>
#include <stdbool.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Gravity Demo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //---------------------------------------
    // Player
    //---------------------------------------

    SDL_FRect player;

    player.x = 375;
    player.y = 50;
    player.w = 50;
    player.h = 50;

    //---------------------------------------
    // Ground
    //---------------------------------------

    SDL_FRect ground;

    ground.x = 0;
    ground.y = 550;
    ground.w = 800;
    ground.h = 50;

    //---------------------------------------
    // Physics
    //---------------------------------------

    float velocityY = 0.0f;

    const float gravity = 981.0f; // pixels/sec²

    Uint64 previousCounter = SDL_GetPerformanceCounter();

    bool running = true;

    while (running)
    {
        //---------------------------------------
        // Delta time
        //---------------------------------------

        Uint64 currentCounter = SDL_GetPerformanceCounter();

        float deltaTime =
            (float)(currentCounter - previousCounter)
            / SDL_GetPerformanceFrequency();

        previousCounter = currentCounter;

        //---------------------------------------
        // Events
        //---------------------------------------

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        //---------------------------------------
        // Gravity
        //---------------------------------------

        velocityY += gravity * deltaTime;

        //---------------------------------------
        // Move player
        //---------------------------------------

        player.y += velocityY * deltaTime;

        //---------------------------------------
        // Collision with ground
        //---------------------------------------

        if (player.y + player.h > ground.y)
        {
            player.y = ground.y - player.h;
            velocityY = 0.0f;
        }

        //---------------------------------------
        // Draw
        //---------------------------------------

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Ground (green)

        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);
        SDL_RenderFillRectF(renderer, &ground);

        // Player (red)

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRectF(renderer, &player);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}