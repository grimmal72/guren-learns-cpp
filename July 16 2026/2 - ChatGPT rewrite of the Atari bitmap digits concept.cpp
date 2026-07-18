#include <SDL.h>

const int DIGIT_WIDTH = 3;
const int DIGIT_HEIGHT = 5;

const int digits[10][DIGIT_HEIGHT][DIGIT_WIDTH] =
{
    // 0
    {
        {1,1,1},
        {1,0,1},
        {1,0,1},
        {1,0,1},
        {1,1,1}
    },

    // 1
    {
        {0,1,0},
        {1,1,0},
        {0,1,0},
        {0,1,0},
        {1,1,1}
    },

    // 2
    {
        {1,1,1},
        {0,0,1},
        {1,1,1},
        {1,0,0},
        {1,1,1}
    },

    // 3
    {
        {1,1,1},
        {0,0,1},
        {0,1,1},
        {0,0,1},
        {1,1,1}
    },

    // 4
    {
        {1,0,1},
        {1,0,1},
        {1,1,1},
        {0,0,1},
        {0,0,1}
    },

    // 5
    {
        {1,1,1},
        {1,0,0},
        {1,1,1},
        {0,0,1},
        {1,1,1}
    },

    // 6
    {
        {1,1,1},
        {1,0,0},
        {1,1,1},
        {1,0,1},
        {1,1,1}
    },

    // 7
    {
        {1,1,1},
        {0,0,1},
        {0,0,1},
        {0,0,1},
        {0,0,1}
    },

    // 8
    {
        {1,1,1},
        {1,0,1},
        {1,1,1},
        {1,0,1},
        {1,1,1}
    },

    // 9
    {
        {1,1,1},
        {1,0,1},
        {1,1,1},
        {0,0,1},
        {1,1,1}
    }
};

void DrawDigit(SDL_Renderer* renderer, int digit, int x, int y)
{
    for (int row = 0; row < DIGIT_HEIGHT; row++)
    {
        for (int col = 0; col < DIGIT_WIDTH; col++)
        {
            if (digits[digit][row][col])
            {
                SDL_RenderDrawPoint(renderer, x + col, y + row);
            }
        }
    }
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Digits",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        640,
        480,
        0);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetScale(renderer, 20, 20);

    SDL_SetRenderDrawColor(renderer, 40, 20, 80, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 220, 180, 60, 255);

    DrawDigit(renderer, 9, 5, 3);

    SDL_RenderPresent(renderer);

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}