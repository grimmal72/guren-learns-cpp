#include <SDL.h>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int PLAYER_SIZE = 40;
const int PLAYER_SPEED = 4;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Collision Demo",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Player
    SDL_Rect player =
    {
        150,
        150,
        PLAYER_SIZE,
        PLAYER_SIZE
    };

    // Walls
    SDL_Rect walls[] =
    {
        // Top
        {100,100,600,40},

        // Bottom
        {100,460,600,40},

        // Left
        {100,100,40,400},

        // Right
        {660,100,40,400}
    };

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        SDL_Rect nextPlayer = player;

        if (keys[SDL_SCANCODE_LEFT])
            nextPlayer.x -= PLAYER_SPEED;

        if (keys[SDL_SCANCODE_RIGHT])
            nextPlayer.x += PLAYER_SPEED;

        if (keys[SDL_SCANCODE_UP])
            nextPlayer.y -= PLAYER_SPEED;

        if (keys[SDL_SCANCODE_DOWN])
            nextPlayer.y += PLAYER_SPEED;

        bool collided = false;

        for (int i = 0; i < 4; i++)
        {
            if (SDL_HasIntersection(&nextPlayer, &walls[i]))
            {
                collided = true;
                break;
            }
        }

        if (!collided)
            player = nextPlayer;

        // Draw
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Walls (green)
        SDL_SetRenderDrawColor(renderer, 0, 200, 0, 255);

        for (int i = 0; i < 4; i++)
            SDL_RenderFillRect(renderer, &walls[i]);

        // Player (red)
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}