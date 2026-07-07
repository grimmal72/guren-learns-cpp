#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <vector>
#include <cstdio>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FRAME_COUNT = 8;      // however many frame_001.png ... frame_010.png you have
const int FRAME_WIDTH = 220;
const int FRAME_HEIGHT = 220;
const int ANIMATION_FPS = 12;    // how fast the GIF should play back

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;

// One texture per frame
std::vector<SDL_Texture*> gFrames;

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gWindow = SDL_CreateWindow("GIF Playback", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return false;
    }

    return true;
}

SDL_Texture* loadTexture(const std::string& path)
{
    SDL_Texture* newTexture = IMG_LoadTexture(gRenderer, path.c_str());
    if (newTexture == nullptr)
    {
        printf("Unable to load texture %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    return newTexture;
}

bool loadMedia()
{
    for (int i = 0; i < FRAME_COUNT; ++i)
    {
        char path[64];
        snprintf(path, sizeof(path), "frame_%03d.png", i + 1); // frame_001.png, frame_002.png, ...

        SDL_Texture* tex = loadTexture(path);
        if (tex == nullptr)
        {
            return false;
        }
        gFrames.push_back(tex);
    }
    return true;
}

void close()
{
    for (SDL_Texture* tex : gFrames)
    {
        SDL_DestroyTexture(tex);
    }
    gFrames.clear();

    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gRenderer = nullptr;
    gWindow = nullptr;

    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char* args[])
{
    if (!init()) return 1;
    if (!loadMedia())
    {
        close();
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    int currentFrame = 0;
    Uint32 frameDelayMs = 1000 / ANIMATION_FPS;
    Uint32 lastFrameTime = SDL_GetTicks();

    SDL_Rect destRect = { (SCREEN_WIDTH - FRAME_WIDTH) / 2,
                           (SCREEN_HEIGHT - FRAME_HEIGHT) / 2,
                           FRAME_WIDTH, FRAME_HEIGHT };

    while (!quit)
    {
        while (SDL_PollEvent(&e) != 0)
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
        }

        // advance frame based on elapsed time, not per-loop-iteration
        Uint32 currentTime = SDL_GetTicks();
        if (currentTime - lastFrameTime >= frameDelayMs)
        {
            currentFrame = (currentFrame + 1) % FRAME_COUNT; // loop back to 0
            lastFrameTime = currentTime;
        }

        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_RenderClear(gRenderer);

        SDL_RenderCopy(gRenderer, gFrames[currentFrame], nullptr, &destRect);

        SDL_RenderPresent(gRenderer);
    }

    close();
    return 0;
}