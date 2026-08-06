#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    // Initialize SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "SDL_mixer could not initialize! Mix_Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    // Load the sound chunk
    Mix_Chunk* sound = Mix_LoadWAV("DraculaLaugh.wav");
    if (sound == nullptr) {
        std::cerr << "Failed to load sound! Mix_Error: " << Mix_GetError() << std::endl;
        return 1;
    }

    bool quit = false;
    SDL_Event e;

    Uint32 startTime = SDL_GetTicks();
    const Uint32 delayMs = 1000; // 1 second(s) 
    bool hasPlayed = false;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }

        // Check elapsed time
        if (!hasPlayed && (SDL_GetTicks() - startTime >= delayMs)) {
            Mix_PlayChannel(-1, sound, 0); // -1 = first free channel, 0 = play once
            hasPlayed = true;
        }

        SDL_Delay(16); // roughly ~60 FPS loop
    }

    // Cleanup
    Mix_FreeChunk(sound);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}