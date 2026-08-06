// Tile-based movement demo (Shining Force / FFT-style, but flat Cartesian, not isometric)
//
// Core design: the player is stored as GRID coordinates (col, row), NOT pixel
// coordinates. Pixel position is only computed at draw time (col * TILE_SIZE).
// This is what makes "snap to tile" movement trivial - there's no interpolation
// or sub-tile state to manage at all. It's either on tile (3,4) or it isn't.
//
// Movement is king-style (8 directions, 1 tile per hop) to match your spec.
// SDL_KEYDOWN combined with e.key.repeat == 0 means each physical key press
// produces exactly one hop, not a stream of repeats while held. Diagonal
// movement works by checking SDL_GetKeyboardState() for currently-held keys
// at the moment a *new* key goes down - e.g. hold Up, then tap Right, and
// since both are down when the Right keydown fires, you get a diagonal hop.

#include <SDL.h>
#include <iostream>

const int TILE_SIZE = 40;
const int GRID_DIM = 8;                         // 8x8 chess board
const int WINDOW_SIZE = TILE_SIZE * GRID_DIM;    // 320x320, square window

struct GridPos {
    int col;
    int row;
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Tile Movement",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_SIZE, WINDOW_SIZE,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Needed so the light-green tile's alpha channel actually blends
    // against the white background instead of just being ignored.
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    GridPos player = { 0, 0 }; // start at top-left tile

    bool running = true;
    SDL_Event e;

    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN && e.key.repeat == 0) {

                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;
                    continue;
                }

                // Read full keyboard state (not just this event's key) so
                // that two arrows held together resolve to a diagonal.
                const Uint8* keys = SDL_GetKeyboardState(nullptr);

                int dx = 0;
                int dy = 0;

                if (keys[SDL_SCANCODE_LEFT])  dx -= 1;
                if (keys[SDL_SCANCODE_RIGHT]) dx += 1;
                if (keys[SDL_SCANCODE_UP])    dy -= 1;
                if (keys[SDL_SCANCODE_DOWN])  dy += 1;

                if (dx != 0 || dy != 0) {
                    int newCol = player.col + dx;
                    int newRow = player.row + dy;

                    // Clamp to the board - this is the entire "collision"
                    // system since there are no other pieces on it.
                    if (newCol >= 0 && newCol < GRID_DIM &&
                        newRow >= 0 && newRow < GRID_DIM) {
                        player.col = newCol;
                        player.row = newRow;
                    }
                }
            }
        }

        // --- Draw ---
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (int row = 0; row < GRID_DIM; row++) {
            for (int col = 0; col < GRID_DIM; col++) {
                SDL_Rect tile = {
                    col * TILE_SIZE,
                    row * TILE_SIZE,
                    TILE_SIZE,
                    TILE_SIZE
                };

                bool isGreen = ((col + row) % 2) == 1;

                if (isGreen) {
                    // Light green, partially transparent, blended over white
                    SDL_SetRenderDrawColor(renderer, 144, 238, 144, 120);
                }
                else {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                }

                SDL_RenderFillRect(renderer, &tile);
            }
        }

        // Player: solid red 40x40 tile, position derived from grid coords
        SDL_Rect playerRect = {
            player.col * TILE_SIZE,
            player.row * TILE_SIZE,
            TILE_SIZE,
            TILE_SIZE
        };
        SDL_SetRenderDrawColor(renderer, 220, 20, 20, 255);
        SDL_RenderFillRect(renderer, &playerRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}