#include <SDL.h>
#include <cstdlib>
#include <ctime>

const int SCREEN_W = 800;
const int SCREEN_H = 600;
const int SQUARE_SIZE = 40;
const int SPEED = 3; // pixels per frame
const int DIRECTION_CHANGE_MS = 1000; // how often it picks a new direction

enum class State
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

State pickRandomState()
{
    int randomNumber = rand() % 4;
    switch (randomNumber)
    {
    case 0: return State::UP;
    case 1: return State::DOWN;
    case 2: return State::LEFT;
    default: return State::RIGHT;
    }
}

int main(int argc, char* argv[])
{
    srand((unsigned int)time(nullptr));

    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Wandering Square",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED
    );

    // Square starts in the middle of the screen
    SDL_Rect square;
    square.x = SCREEN_W / 2 - SQUARE_SIZE / 2;
    square.y = SCREEN_H / 2 - SQUARE_SIZE / 2;
    square.w = SQUARE_SIZE;
    square.h = SQUARE_SIZE;

    State currentState = pickRandomState();
    Uint32 lastChangeTime = SDL_GetTicks();

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        // Time to pick a new random direction?
        Uint32 now = SDL_GetTicks();
        if (now - lastChangeTime >= DIRECTION_CHANGE_MS)
        {
            currentState = pickRandomState();
            lastChangeTime = now;
        }

        // Move based on current state
        switch (currentState)
        {
        case State::UP:
            square.y -= SPEED;
            break;
        case State::DOWN:
            square.y += SPEED;
            break;
        case State::LEFT:
            square.x -= SPEED;
            break;
        case State::RIGHT:
            square.x += SPEED;
            break;
        }

        // Draw
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 220, 60, 60, 255);
        SDL_RenderFillRect(renderer, &square);

        SDL_RenderPresent(renderer);

        SDL_Delay(16); // roughly 60 FPS
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}