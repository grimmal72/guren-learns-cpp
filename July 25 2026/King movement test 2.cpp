// tile_king_move.cpp
//
// Tile-based movement demo (Shining Force / FFT-style, but flat Cartesian
// instead of isometric). A single "king" piece hops one tile at a time in
// any of the 8 directions on an 8x8 checkerboard. No scrolling, fixed window.
//
// Controls:
//   Arrows / WASD  -> cardinal move (N/S/E/W)
//   Q E            -> diagonal move (NW / NE)
//   Z C            -> diagonal move (SW / SE)
//   Esc            -> quit
//
// Build (MSVC / Visual Studio):
//   Link against SDL2.lib SDL2main.lib, include SDL.h, copy SDL2.dll next to exe.
//
// Build (g++ / MSYS2 / Linux):
//   g++ tile_king_move.cpp -o tile_king_move `sdl2-config --cflags --libs`

#include <SDL.h>
#include <cstdlib>

// ---------------------------------------------------------------------------
// Board / tile constants
// ---------------------------------------------------------------------------
constexpr int BOARD_COLS = 8;
constexpr int BOARD_ROWS = 8;
constexpr int TILE_SIZE = 40;                     // px, per your spec
constexpr int WIN_W = BOARD_COLS * TILE_SIZE;      // 320
constexpr int WIN_H = BOARD_ROWS * TILE_SIZE;      // 320

// Hop animation timing
constexpr Uint32 HOP_DURATION_MS = 140;
constexpr int     HOP_ARC_HEIGHT = 10;            // px, purely visual "lift"

// ---------------------------------------------------------------------------
// Character / grid state
// ---------------------------------------------------------------------------
struct GridPos {
    int col = 3;
    int row = 3;
};

struct Character {
    GridPos logical;       // authoritative tile the character occupies
    GridPos animFrom;      // tile we're hopping FROM (for interpolation)
    bool    isHopping = false;
    Uint32  hopStartTicks = 0;
};

// Clamp a proposed move to board bounds. Returns true if the move is legal.
bool tryStep(const Character& c, int dCol, int dRow, GridPos& outTarget) {
    int newCol = c.logical.col + dCol;
    int newRow = c.logical.row + dRow;
    if (newCol < 0 || newCol >= BOARD_COLS) return false;
    if (newRow < 0 || newRow >= BOARD_ROWS) return false;
    outTarget = { newCol, newRow };
    return true;
}

// Begin a hop toward target (assumes target already validated by tryStep).
void beginHop(Character& c, GridPos target, Uint32 nowTicks) {
    c.animFrom = c.logical;   // where we're hopping from, for rendering
    c.logical = target;      // logical state updates immediately —
    // game logic (e.g. "what tile am I on") is
    // never blocked on animation finishing
    c.isHopping = true;
    c.hopStartTicks = nowTicks;
}

// Simple ease (smoothstep) so the hop isn't linear/robotic.
float smoothstep(float t) {
    return t * t * (3.0f - 2.0f * t);
}

// Compute current render-space pixel position for the character, accounting
// for an in-progress hop animation.
void getRenderPixelPos(Character& c, Uint32 nowTicks, float& outX, float& outY) {
    if (!c.isHopping) {
        outX = (float)(c.logical.col * TILE_SIZE);
        outY = (float)(c.logical.row * TILE_SIZE);
        return;
    }

    Uint32 elapsed = nowTicks - c.hopStartTicks;
    if (elapsed >= HOP_DURATION_MS) {
        c.isHopping = false;
        outX = (float)(c.logical.col * TILE_SIZE);
        outY = (float)(c.logical.row * TILE_SIZE);
        return;
    }

    float t = (float)elapsed / (float)HOP_DURATION_MS;
    float e = smoothstep(t);

    float fromX = (float)(c.animFrom.col * TILE_SIZE);
    float fromY = (float)(c.animFrom.row * TILE_SIZE);
    float toX = (float)(c.logical.col * TILE_SIZE);
    float toY = (float)(c.logical.row * TILE_SIZE);

    outX = fromX + (toX - fromX) * e;
    outY = fromY + (toY - fromY) * e;

    // Add a small vertical arc so it reads as a "hop" rather than a slide.
    // sin(pi * t) goes 0 -> 1 -> 0 across the animation.
    float arc = (float)HOP_ARC_HEIGHT * sinf((float)M_PI * t);
    outY -= arc;
}

// ---------------------------------------------------------------------------
// Rendering
// ---------------------------------------------------------------------------
void drawBoard(SDL_Renderer* renderer) {
    for (int row = 0; row < BOARD_ROWS; ++row) {
        for (int col = 0; col < BOARD_COLS; ++col) {
            SDL_Rect tile = { col * TILE_SIZE, row * TILE_SIZE, TILE_SIZE, TILE_SIZE };

            // Alternate white / light alpha green, checkerboard pattern.
            if ((col + row) % 2 == 0) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // white
            }
            else {
                // "light alpha green" — since the background is opaque, we
                // pre-blend a translucent green over white by hand here.
                // (170,220,170) reads as a soft green if you want a true
                // alpha-blended tile instead, see the blended-tile note below.
                SDL_SetRenderDrawColor(renderer, 190, 230, 190, 255);
            }
            SDL_RenderFillRect(renderer, &tile);
        }
    }
}

void drawCharacter(SDL_Renderer* renderer, float x, float y) {
    SDL_Rect r = { (int)x, (int)y, TILE_SIZE, TILE_SIZE };
    SDL_SetRenderDrawColor(renderer, 220, 30, 30, 255); // red
    SDL_RenderFillRect(renderer, &r);

    // Thin border so the piece reads clearly against the tile beneath it.
    SDL_SetRenderDrawColor(renderer, 120, 0, 0, 255);
    SDL_RenderDrawRect(renderer, &r);
}

// ---------------------------------------------------------------------------
// Main
// ---------------------------------------------------------------------------
int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Tile King Move",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WIN_W, WIN_H,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    Character player;
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
                }

                // Ignore movement input while a hop is still animating —
                // prevents input queueing/desync and keeps moves feeling
                // discrete, one-tile-at-a-time (like a chess king, not a
                // free-roaming sprite).
                if (!player.isHopping) {
                    int dCol = 0, dRow = 0;
                    bool moveRequested = true;

                    switch (e.key.keysym.sym) {
                        // Cardinal: arrows / WASD
                    case SDLK_UP:    case SDLK_w: dRow = -1; break;
                    case SDLK_DOWN:  case SDLK_s: dRow = 1; break;
                    case SDLK_LEFT:  case SDLK_a: dCol = -1; break;
                    case SDLK_RIGHT: case SDLK_d: dCol = 1; break;

                        // Diagonals: Q/E (up-left, up-right), Z/C (down-left, down-right)
                    case SDLK_q: dCol = -1; dRow = -1; break;
                    case SDLK_e: dCol = 1; dRow = -1; break;
                    case SDLK_z: dCol = -1; dRow = 1; break;
                    case SDLK_c: dCol = 1; dRow = 1; break;

                    default: moveRequested = false; break;
                    }

                    if (moveRequested) {
                        GridPos target;
                        if (tryStep(player, dCol, dRow, target)) {
                            beginHop(player, target, SDL_GetTicks());
                        }
                        // If tryStep fails (board edge), we simply do nothing —
                        // no animation starts, piece stays put.
                    }
                }
            }
        }

        Uint32 now = SDL_GetTicks();
        float px, py;
        getRenderPixelPos(player, now, px, py);

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        drawBoard(renderer);
        drawCharacter(renderer, px, py);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}