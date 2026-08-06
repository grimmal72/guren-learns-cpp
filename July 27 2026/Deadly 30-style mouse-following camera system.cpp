// Deadly 30 - style look-ahead camera demo
// ---------------------------------------------------------------
// Stationary square "player" in the middle of a wide world.
// The mouse controls a drawn reticle. When the mouse crosses a
// deadzone threshold near the left/right edge of the window, the
// camera eases toward that side (revealing more world ahead of the
// aim direction) instead of the player moving. It gradates smoothly
// between fully-centered and max look-ahead, and eases back to
// center as the mouse returns toward the middle.
// ---------------------------------------------------------------

#include <SDL.h>
#include <cmath>
#include <cstdlib>
#include <vector>

const int WINDOW_WIDTH_PIXELS = 1280;
const int WINDOW_HEIGHT_PIXELS = 720;

// The world is much wider than the window so there is somewhere for
// the camera to pan to.
const int WORLD_WIDTH_PIXELS = 4000;
const int WORLD_HEIGHT_PIXELS = 720;

// --- Look-ahead camera tuning ---------------------------------------
// Fraction of the distance from screen-center to screen-edge that
// counts as "dead" with no camera shift at all. 0.15 means the middle
// 15% of each half of the window does nothing; past that, the camera
// shift starts ramping up.
const float DEADZONE_FRACTION_OF_HALF_WIDTH = 0.15f;

// Maximum number of pixels the camera will shift away from the player
// when the mouse is pinned at the extreme left/right edge of the window.
const float MAX_CAMERA_LOOKAHEAD_PIXELS = 420.0f;

// How quickly the camera eases toward its target look-ahead offset,
// in "fraction closed per second". Higher = snappier, lower =
// floatier. This is what makes the transition gradate smoothly
// instead of snapping instantly to a new position.
const float CAMERA_EASE_SPEED_PER_SECOND = 4.0f;

const int PLAYER_SQUARE_SIZE_PIXELS = 48;

const int RETICLE_OUTER_RING_RADIUS_PIXELS = 16;
const int RETICLE_CENTER_DOT_RADIUS_PIXELS = 4;

struct WorldPosition
{
    float x;
    float y;
};

// Simple circle outline, drawn point-by-point (no SDL2_gfx dependency,
// so nothing to fight with x64/USE_MMX build flags).
void drawCircleOutline(SDL_Renderer* renderer, int centerX, int centerY, int radius)
{
    int circleX = radius;
    int circleY = 0;
    int decisionError = 0;

    while (circleX >= circleY)
    {
        SDL_RenderDrawPoint(renderer, centerX + circleX, centerY + circleY);
        SDL_RenderDrawPoint(renderer, centerX + circleY, centerY + circleX);
        SDL_RenderDrawPoint(renderer, centerX - circleY, centerY + circleX);
        SDL_RenderDrawPoint(renderer, centerX - circleX, centerY + circleY);
        SDL_RenderDrawPoint(renderer, centerX - circleX, centerY - circleY);
        SDL_RenderDrawPoint(renderer, centerX - circleY, centerY - circleX);
        SDL_RenderDrawPoint(renderer, centerX + circleY, centerY - circleX);
        SDL_RenderDrawPoint(renderer, centerX + circleX, centerY - circleY);

        if (decisionError <= 0)
        {
            circleY += 1;
            decisionError += 2 * circleY + 1;
        }
        if (decisionError > 0)
        {
            circleX -= 1;
            decisionError -= 2 * circleX + 1;
        }
    }
}

void drawReticleAtScreenPosition(SDL_Renderer* renderer, int mouseScreenX, int mouseScreenY)
{
    SDL_SetRenderDrawColor(renderer, 255, 60, 60, 255);

    const int tickGapFromRing = 4;
    const int tickLengthOutward = 8;

    drawCircleOutline(renderer, mouseScreenX, mouseScreenY, RETICLE_OUTER_RING_RADIUS_PIXELS);

    // Crosshair ticks poking outward from the ring, leaving a gap
    // around the ring itself so it doesn't look like a solid plus sign.
    SDL_RenderDrawLine(renderer,
        mouseScreenX - RETICLE_OUTER_RING_RADIUS_PIXELS - tickLengthOutward, mouseScreenY,
        mouseScreenX - RETICLE_OUTER_RING_RADIUS_PIXELS + tickGapFromRing, mouseScreenY);
    SDL_RenderDrawLine(renderer,
        mouseScreenX + RETICLE_OUTER_RING_RADIUS_PIXELS - tickGapFromRing, mouseScreenY,
        mouseScreenX + RETICLE_OUTER_RING_RADIUS_PIXELS + tickLengthOutward, mouseScreenY);
    SDL_RenderDrawLine(renderer,
        mouseScreenX, mouseScreenY - RETICLE_OUTER_RING_RADIUS_PIXELS - tickLengthOutward,
        mouseScreenX, mouseScreenY - RETICLE_OUTER_RING_RADIUS_PIXELS + tickGapFromRing);
    SDL_RenderDrawLine(renderer,
        mouseScreenX, mouseScreenY + RETICLE_OUTER_RING_RADIUS_PIXELS - tickGapFromRing,
        mouseScreenX, mouseScreenY + RETICLE_OUTER_RING_RADIUS_PIXELS + tickLengthOutward);

    // Small filled center dot.
    for (int offsetY = -RETICLE_CENTER_DOT_RADIUS_PIXELS; offsetY <= RETICLE_CENTER_DOT_RADIUS_PIXELS; ++offsetY)
    {
        for (int offsetX = -RETICLE_CENTER_DOT_RADIUS_PIXELS; offsetX <= RETICLE_CENTER_DOT_RADIUS_PIXELS; ++offsetX)
        {
            if (offsetX * offsetX + offsetY * offsetY <= RETICLE_CENTER_DOT_RADIUS_PIXELS * RETICLE_CENTER_DOT_RADIUS_PIXELS)
                SDL_RenderDrawPoint(renderer, mouseScreenX + offsetX, mouseScreenY + offsetY);
        }
    }
}

float clampFloat(float value, float minimum, float maximum)
{
    if (value < minimum) return minimum;
    if (value > maximum) return maximum;
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
        "Look-Ahead Camera Demo",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH_PIXELS, WINDOW_HEIGHT_PIXELS,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Player is a stationary square, fixed in world space at the
    // horizontal and vertical middle of the world.
    WorldPosition playerWorldPosition = {
        WORLD_WIDTH_PIXELS / 2.0f,
        WORLD_HEIGHT_PIXELS / 2.0f
    };

    // Scatter some "zombie" marker squares around the world so panning
    // actually shows you something new to the left/right of the player.
    std::vector<SDL_Rect> zombieWorldRects;
    srand(1234);
    for (int i = 0; i < 24; ++i)
    {
        SDL_Rect zombieRect;
        zombieRect.w = 32;
        zombieRect.h = 32;
        zombieRect.x = (rand() % (WORLD_WIDTH_PIXELS - 200)) + 100;
        zombieRect.y = (rand() % (WORLD_HEIGHT_PIXELS - 200)) + 100;
        zombieWorldRects.push_back(zombieRect);
    }

    // Camera state: cameraWorldX/cameraWorldY is the world-space
    // coordinate that appears at the top-left corner of the window.
    float cameraWorldX = playerWorldPosition.x - WINDOW_WIDTH_PIXELS / 2.0f;
    float cameraWorldY = playerWorldPosition.y - WINDOW_HEIGHT_PIXELS / 2.0f;

    // currentCameraLookaheadPixels eases toward targetCameraLookaheadPixels
    // every frame, which is what produces the smooth gradation instead
    // of an instant snap when the mouse crosses the deadzone.
    float currentCameraLookaheadPixels = 0.0f;

    bool isRunning = true;
    Uint64 previousFrameTimestamp = SDL_GetPerformanceCounter();

    while (isRunning)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT) isRunning = false;
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) isRunning = false;
        }

        Uint64 currentFrameTimestamp = SDL_GetPerformanceCounter();
        float deltaTimeSeconds =
            (float)(currentFrameTimestamp - previousFrameTimestamp) / (float)SDL_GetPerformanceFrequency();
        previousFrameTimestamp = currentFrameTimestamp;
        if (deltaTimeSeconds > 0.1f) deltaTimeSeconds = 0.1f; // clamp huge spikes (e.g. window drag)

        int mouseScreenX, mouseScreenY;
        SDL_GetMouseState(&mouseScreenX, &mouseScreenY); // window-space, not world-space

        // --- Compute target look-ahead based on mouse position -----
        float halfWindowWidth = WINDOW_WIDTH_PIXELS / 2.0f;
        float mouseOffsetFromCenterPixels = (mouseScreenX - halfWindowWidth); // -halfWindowWidth .. +halfWindowWidth
        float mouseOffsetFromCenterNormalized = mouseOffsetFromCenterPixels / halfWindowWidth; // -1 .. 1

        float mouseDirectionSign = (mouseOffsetFromCenterNormalized < 0.0f) ? -1.0f : 1.0f;
        float mouseDistanceFromCenterFraction = fabsf(mouseOffsetFromCenterNormalized);

        float targetCameraLookaheadPixels = 0.0f;
        if (mouseDistanceFromCenterFraction > DEADZONE_FRACTION_OF_HALF_WIDTH)
        {
            float rampedFraction =
                (mouseDistanceFromCenterFraction - DEADZONE_FRACTION_OF_HALF_WIDTH) /
                (1.0f - DEADZONE_FRACTION_OF_HALF_WIDTH);
            rampedFraction = clampFloat(rampedFraction, 0.0f, 1.0f);
            targetCameraLookaheadPixels = mouseDirectionSign * rampedFraction * MAX_CAMERA_LOOKAHEAD_PIXELS;
        }

        // Ease current look-ahead toward target -> smooth gradation over time.
        float easeAmountThisFrame = clampFloat(CAMERA_EASE_SPEED_PER_SECOND * deltaTimeSeconds, 0.0f, 1.0f);
        currentCameraLookaheadPixels +=
            (targetCameraLookaheadPixels - currentCameraLookaheadPixels) * easeAmountThisFrame;

        // Camera normally centers on the (stationary) player, offset
        // by the current look-ahead amount.
        float desiredCameraWorldX =
            (playerWorldPosition.x - WINDOW_WIDTH_PIXELS / 2.0f) + currentCameraLookaheadPixels;
        float desiredCameraWorldY = playerWorldPosition.y - WINDOW_HEIGHT_PIXELS / 2.0f;

        // Clamp camera so we never show past the edges of the world.
        cameraWorldX = clampFloat(desiredCameraWorldX, 0.0f, (float)(WORLD_WIDTH_PIXELS - WINDOW_WIDTH_PIXELS));
        cameraWorldY = clampFloat(desiredCameraWorldY, 0.0f, (float)(WORLD_HEIGHT_PIXELS - WINDOW_HEIGHT_PIXELS));

        // --- Draw --------------------------------------------------
        SDL_SetRenderDrawColor(renderer, 20, 22, 28, 255);
        SDL_RenderClear(renderer);

        // Faint vertical gridlines every 200 world-pixels, so you can
        // visually see the camera sliding relative to the world.
        SDL_SetRenderDrawColor(renderer, 40, 44, 54, 255);
        for (int worldGridlineX = 0; worldGridlineX < WORLD_WIDTH_PIXELS; worldGridlineX += 200)
        {
            int screenGridlineX = (int)(worldGridlineX - cameraWorldX);
            if (screenGridlineX >= 0 && screenGridlineX <= WINDOW_WIDTH_PIXELS)
                SDL_RenderDrawLine(renderer, screenGridlineX, 0, screenGridlineX, WINDOW_HEIGHT_PIXELS);
        }

        // Zombies (world-space -> screen-space via camera offset).
        SDL_SetRenderDrawColor(renderer, 90, 160, 90, 255);
        for (auto& zombieWorldRect : zombieWorldRects)
        {
            SDL_Rect zombieScreenRect = zombieWorldRect;
            zombieScreenRect.x = (int)(zombieWorldRect.x - cameraWorldX);
            zombieScreenRect.y = (int)(zombieWorldRect.y - cameraWorldY);
            if (zombieScreenRect.x + zombieScreenRect.w >= 0 && zombieScreenRect.x <= WINDOW_WIDTH_PIXELS)
                SDL_RenderFillRect(renderer, &zombieScreenRect);
        }

        // Player square (world-space -> screen-space via camera offset).
        SDL_Rect playerScreenRect;
        playerScreenRect.w = PLAYER_SQUARE_SIZE_PIXELS;
        playerScreenRect.h = PLAYER_SQUARE_SIZE_PIXELS;
        playerScreenRect.x = (int)(playerWorldPosition.x - PLAYER_SQUARE_SIZE_PIXELS / 2.0f - cameraWorldX);
        playerScreenRect.y = (int)(playerWorldPosition.y - PLAYER_SQUARE_SIZE_PIXELS / 2.0f - cameraWorldY);
        SDL_SetRenderDrawColor(renderer, 230, 200, 60, 255);
        SDL_RenderFillRect(renderer, &playerScreenRect);

        // Reticle follows the mouse directly in window/screen space
        // (it is not affected by the camera offset).
        drawReticleAtScreenPosition(renderer, mouseScreenX, mouseScreenY);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}