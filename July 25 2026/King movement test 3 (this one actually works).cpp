// tile_movement.cpp
//
// Tile-based "king move" character system, SDL2.
// - 8x8 board, 40px tiles, 320x320 window (no scrolling)
// - Character moves exactly 1 tile per keypress, in any of 8 directions
// - Checkerboard background: white / light alpha green
//
// Controls:
//   Cardinal:  Arrow keys or W A S D
//   Diagonals: Q (up-left)  E (up-right)
//              Z (down-left) C (down-right)
//   Esc to quit
//
// Build (matches your existing VS2022 SDL2 setup):
//   - Link SDL2.lib, SDL2main.lib
//   - Requires SDL2.dll alongside the exe
//
// Or on the command line (e.g. MSYS2/mingw):
//   g++ tile_movement.cpp -o tile_movement -lSDL2main -lSDL2

#include <SDL.h>

// ---------------------------------------------------------------------
// Board / window constants
// ---------------------------------------------------------------------
constexpr int TILE_SIZE = 40;
constexpr int BOARD_TILES = 8;
constexpr int WINDOW_SIZE = TILE_SIZE * BOARD_TILES; // 320

// ---------------------------------------------------------------------
// Convert a grid coordinate to a screen-space SDL_Rect
// ---------------------------------------------------------------------
SDL_Rect gridToRect(int gridX, int gridY)
{
    SDL_Rect r;
    r.x = gridX * TILE_SIZE;
    r.y = gridY * TILE_SIZE;
    r.w = TILE_SIZE;
    r.h = TILE_SIZE;
    return r;
}

// ---------------------------------------------------------------------
// Draw the alternating checkerboard background
// ---------------------------------------------------------------------
void drawBoard(SDL_Renderer* renderer)
{
    // Base clear: white
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);

    // Overlay light-green tiles with alpha blending
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer, 100, 220, 120, 110); // light alpha green

    for (int y = 0; y < BOARD_TILES; ++y)
    {
        for (int x = 0; x < BOARD_TILES; ++x)
        {
            // Standard checkerboard parity check
            if ((x + y) % 2 == 1)
            {
                SDL_Rect tile = gridToRect(x, y);
                SDL_RenderFillRect(renderer, &tile);
            }
        }
    }
}

// ---------------------------------------------------------------------
// Draw the player as a solid red square
// ---------------------------------------------------------------------
void drawPlayer(SDL_Renderer* renderer, int playerX, int playerY)
{
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_NONE);
    SDL_SetRenderDrawColor(renderer, 220, 30, 30, 255);
    SDL_Rect playerRect = gridToRect(playerX, playerY);
    SDL_RenderFillRect(renderer, &playerRect);
}

// ---------------------------------------------------------------------
// Clamp helper so the player can't walk off the board
// ---------------------------------------------------------------------
int clampToBoard(int value)
{
    if (value < 0) return 0;
    if (value > BOARD_TILES - 1) return BOARD_TILES - 1;
    return value;
}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Tile Movement Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_SIZE, WINDOW_SIZE,
        SDL_WINDOW_SHOWN
    );

    if (window == nullptr)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (renderer == nullptr)
    {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Player starts roughly in the middle of the board
    int playerGridX = BOARD_TILES / 2;
    int playerGridY = BOARD_TILES / 2;

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
            else if (event.type == SDL_KEYDOWN && event.key.repeat == 0)
            {
                // event.key.repeat == 0 ignores OS key-repeat, so holding
                // a key down doesn't fire multiple hops. Combined with
                // reacting only to KEYDOWN (not polled state), this gives
                // exactly one tile of movement per physical key press.

                int dx = 0;
                int dy = 0;

                switch (event.key.keysym.sym)
                {
                    // Cardinal directions
                case SDLK_UP:    case SDLK_w: dy = -1; break;
                case SDLK_DOWN:  case SDLK_s: dy = 1; break;
                case SDLK_LEFT:  case SDLK_a: dx = -1; break;
                case SDLK_RIGHT: case SDLK_d: dx = 1; break;

                    // Diagonals
                case SDLK_q: dx = -1; dy = -1; break; // up-left
                case SDLK_e: dx = 1; dy = -1; break; // up-right
                case SDLK_z: dx = -1; dy = 1; break; // down-left
                case SDLK_c: dx = 1; dy = 1; break; // down-right

                case SDLK_ESCAPE:
                    running = false;
                    break;

                default:
                    break;
                }

                playerGridX = clampToBoard(playerGridX + dx);
                playerGridY = clampToBoard(playerGridY + dy);
            }
        }

        drawBoard(renderer);
        drawPlayer(renderer, playerGridX, playerGridY);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}