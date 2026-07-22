// healthbar.cpp
// A long, flat red health bar. The left edge (origin) stays fixed.
// Each mouse click deals damage, shrinking the bar from the right,
// like an RPG/action-game health bar draining.

#include <SDL.h>
#include <iostream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int healthBarX = 100;  // fixed left edge (origin)
int healthBarY = 280;  // fixed vertical position
int healthBarHeight = 40;   // flat

int maxHealth = 600;  // full bar width, in "health points" (1 hp = 1 px)
int damagePerHit = 5; // width removed from the right per click
int minHealth = 0;    // don't let it go negative (bar is "dead")

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Health Bar Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window)
    {
        std::cout << "CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer)
    {
        std::cout << "CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int currentHealth = maxHealth;
    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                // Take damage: only the bar's width shrinks, HEALTHBAR_X never moves.
                currentHealth -= damagePerHit;
                if (currentHealth < minHealth)
                    currentHealth = minHealth;
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255); // Generic grey. Could be anything.
        SDL_RenderClear(renderer);

        SDL_Rect healthBar;
        healthBar.x = healthBarX;       // origin stays put
        healthBar.y = healthBarY;
        healthBar.w = currentHealth;     // this is what drains
        healthBar.h = healthBarHeight;

        SDL_SetRenderDrawColor(renderer, 220, 30, 30, 255); // Red color for the health bar.
        SDL_RenderFillRect(renderer, &healthBar);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}