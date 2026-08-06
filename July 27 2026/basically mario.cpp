// Simple SDL2 side-scroller: rect player, flat ground, clamped camera.
// No jumping, no blocks, no enemies - just left/right movement with a
// world boundary the camera and player can't scroll past.

#include <SDL.h>

const int SCREEN_W = 800;
const int SCREEN_H = 600;

const int WORLD_W = 1000;      // total level width - the "eternal ground"
// still has to end somewhere for a demo,
// but you can just bump this way up.
const int GROUND_H = 80;       // height of the ground strip

const int PLAYER_W = 40;
const int PLAYER_H = 60;
const float PLAYER_SPEED = 300.0f; // pixels per second

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Scrolling Rect Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_W, SCREEN_H,
        SDL_WINDOW_SHOWN
    );
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    // Player position is in WORLD space, not screen space.
    float playerX = 100.0f;
    float playerY = SCREEN_H - GROUND_H - PLAYER_H;

    float cameraX = 0.0f;

    bool running = true;
    Uint64 lastTicks = SDL_GetPerformanceCounter();

    while (running) {
        // --- timing ---
        Uint64 now = SDL_GetPerformanceCounter();
        float dt = (float)(now - lastTicks) / SDL_GetPerformanceFrequency();
        lastTicks = now;

        // --- events ---
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
        }

        // --- input / movement ---
        const Uint8* keys = SDL_GetKeyboardState(NULL);
        if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D]) {
            playerX += PLAYER_SPEED * dt;
        }
        if (keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A]) {
            playerX -= PLAYER_SPEED * dt;
        }

        // Clamp player to world bounds
        if (playerX < 0.0f) playerX = 0.0f;
        if (playerX > WORLD_W - PLAYER_W) playerX = WORLD_W - PLAYER_W;

        // Camera follows player, centered, but clamped to world edges
        // so it never shows past the start/end of the level.
        cameraX = playerX - (SCREEN_W / 2.0f) + (PLAYER_W / 2.0f);
        if (cameraX < 0.0f) cameraX = 0.0f;
        if (cameraX > WORLD_W - SCREEN_W) cameraX = WORLD_W - SCREEN_W;

        // --- render ---
        SDL_SetRenderDrawColor(renderer, 92, 148, 252, 255); // sky
        SDL_RenderClear(renderer);

        // Ground - drawn relative to camera. Since it's one flat strip
        // spanning the whole world, we just draw one big rect offset
        // by the camera.
        SDL_Rect ground;
        ground.x = (int)(0 - cameraX);
        ground.y = SCREEN_H - GROUND_H;
        ground.w = WORLD_W;
        ground.h = GROUND_H;
        SDL_SetRenderDrawColor(renderer, 96, 60, 32, 255); // dirt brown
        SDL_RenderFillRect(renderer, &ground);

        // A thin green "grass" cap on top of the ground for a Mario-ish look
        SDL_Rect grass = ground;
        grass.h = 10;
        SDL_SetRenderDrawColor(renderer, 60, 180, 75, 255);
        SDL_RenderFillRect(renderer, &grass);

        // Player - convert world position to screen position
        SDL_Rect playerRect;
        playerRect.x = (int)(playerX - cameraX);
        playerRect.y = (int)playerY;
        playerRect.w = PLAYER_W;
        playerRect.h = PLAYER_H;
        SDL_SetRenderDrawColor(renderer, 220, 40, 40, 255); // Mario-red
        SDL_RenderFillRect(renderer, &playerRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}