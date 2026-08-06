// Why is there a picture of Hitler here? Pretty weird, right? I have this idea I might want to work on in the future that is like the game Deadly 30, where you're shooting down hordes of enemies. But in my game the enemies would be Nazis, and later Hitler. This effect on him is for the intro, where I think there will be a gunshot sound, a bullet hole will appear in front of him, then he will turn to red. There may also be a demon laugh and the red silhouette may sprout wings and fly away, as maybe he'll be a demon guy. 
// -----------------------------------------------------------------------

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>

// ---------------------------------------------------------------------------
// Configuration constants
// ---------------------------------------------------------------------------

const int WINDOW_WIDTH = 373;
const int WINDOW_HEIGHT = 463;

const std::string NORMAL_SPRITE_PATH = "AH.png";      // fully colored sprite
const std::string SILHOUETTE_SPRITE_PATH = "AHsilhouette.png";  // flat red silhouette

const Uint32 HOLD_DURATION_MILLISECONDS = 2000;  // how long to show the normal sprite before fading
const Uint32 FADE_DURATION_MILLISECONDS = 1200;  // how long the silhouette takes to fade to full opacity

// ---------------------------------------------------------------------------
// Loads a PNG file into a hardware-accelerated SDL_Texture.
// Returns nullptr on failure and prints an SDL_image error message.
// ---------------------------------------------------------------------------
SDL_Texture* loadTextureFromFile(SDL_Renderer* renderer, const std::string& filePath)
{
    SDL_Texture* loadedTexture = IMG_LoadTexture(renderer, filePath.c_str());
    if (loadedTexture == nullptr)
    {
        std::cerr << "Failed to load texture \"" << filePath << "\": "
            << IMG_GetError() << std::endl;
    }
    return loadedTexture;
}

// ---------------------------------------------------------------------------
// Given how many milliseconds have elapsed since the fade started, returns
// an alpha value from 0 (fully transparent) to 255 (fully opaque).
// Clamps at the endpoints so the caller doesn't need to worry about overshoot.
// ---------------------------------------------------------------------------
Uint8 calculateSilhouetteAlpha(Uint32 millisecondsSinceFadeStarted)
{
    if (millisecondsSinceFadeStarted >= FADE_DURATION_MILLISECONDS)
    {
        return 255;
    }

    double fadeProgress = static_cast<double>(millisecondsSinceFadeStarted) /
        static_cast<double>(FADE_DURATION_MILLISECONDS);

    return static_cast<Uint8>(fadeProgress * 255.0);
}

int main(int argc, char* argv[])
{
    // -----------------------------------------------------------------
    // Initialize SDL and SDL_image
    // -----------------------------------------------------------------
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        return 1;
    }

    int imageInitFlags = IMG_INIT_PNG;
    if ((IMG_Init(imageInitFlags) & imageInitFlags) != imageInitFlags)
    {
        std::cerr << "IMG_Init failed: " << IMG_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Shinobi Silhouette Fade",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (window == nullptr)
    {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (renderer == nullptr)
    {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // -----------------------------------------------------------------
    // Load the two sprite textures
    // -----------------------------------------------------------------
    SDL_Texture* normalSpriteTexture = loadTextureFromFile(renderer, NORMAL_SPRITE_PATH);
    SDL_Texture* silhouetteSpriteTexture = loadTextureFromFile(renderer, SILHOUETTE_SPRITE_PATH);

    if (normalSpriteTexture == nullptr || silhouetteSpriteTexture == nullptr)
    {
        // Clean up whichever textures did load before exiting.
        if (normalSpriteTexture != nullptr) SDL_DestroyTexture(normalSpriteTexture);
        if (silhouetteSpriteTexture != nullptr) SDL_DestroyTexture(silhouetteSpriteTexture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // This is the key setup step: enable alpha blending on the silhouette
    // texture so that SDL_SetTextureAlphaMod() actually blends it with
    // whatever has already been drawn underneath, instead of just replacing it.
    SDL_SetTextureBlendMode(silhouetteSpriteTexture, SDL_BLENDMODE_BLEND);

    // -----------------------------------------------------------------
    // Main loop
    // -----------------------------------------------------------------
    Uint32 programStartTimeMilliseconds = SDL_GetTicks();
    bool programShouldQuit = false;

    while (!programShouldQuit)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                programShouldQuit = true;
            }
        }

        Uint32 elapsedMilliseconds = SDL_GetTicks() - programStartTimeMilliseconds;

        // Clear the screen each frame.
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // The normal sprite is always drawn first, as the base layer.
        SDL_RenderCopy(renderer, normalSpriteTexture, nullptr, nullptr);

        if (elapsedMilliseconds >= HOLD_DURATION_MILLISECONDS)
        {
            // We are past the hold phase, so we're somewhere in (or past) the fade.
            Uint32 millisecondsSinceFadeStarted = elapsedMilliseconds - HOLD_DURATION_MILLISECONDS;
            Uint8 silhouetteAlpha = calculateSilhouetteAlpha(millisecondsSinceFadeStarted);

            SDL_SetTextureAlphaMod(silhouetteSpriteTexture, silhouetteAlpha);
            SDL_RenderCopy(renderer, silhouetteSpriteTexture, nullptr, nullptr);
        }
        // Else: still in the 2-second hold phase, so only the normal sprite is drawn.

        SDL_RenderPresent(renderer);
    }

    // -----------------------------------------------------------------
    // Cleanup
    // -----------------------------------------------------------------
    SDL_DestroyTexture(silhouetteSpriteTexture);
    SDL_DestroyTexture(normalSpriteTexture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}