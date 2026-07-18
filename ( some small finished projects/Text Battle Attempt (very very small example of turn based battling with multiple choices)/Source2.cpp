#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>
using namespace std;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Window* window = SDL_CreateWindow("Rickroll", 800, 600, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    
    cout << "You are exploring a dark and mysterious cave you found from a hole in the ground next to a fallen tree in the forest.\n\n";


    // Audio
    SDL_AudioSpec spec;
    Uint8* audioBuf = NULL;
    Uint32 audioLen = 0;
    if (!SDL_LoadWAV("Songs/rickroll.wav", &spec, &audioBuf, &audioLen)) {
        printf("Audio failed: %s\n", SDL_GetError());
    }
    else {
        SDL_AudioStream* stream = SDL_OpenAudioDeviceStream(
            SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL
        );
        SDL_PutAudioStreamData(stream, audioBuf, (int)audioLen);
        SDL_ResumeAudioStreamDevice(stream);
    }

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_EVENT_QUIT) running = false;
    }

    SDL_free(audioBuf);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}