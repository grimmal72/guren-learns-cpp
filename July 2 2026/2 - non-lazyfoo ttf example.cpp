//Using SDL and standard IO
#include <stdio.h>

#include <SDL.h>
#include <SDL_ttf.h>

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window =
        SDL_CreateWindow(
            "Text",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            800,
            600,
            SDL_WINDOW_SHOWN
        );

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, 0);

    TTF_Font* font =
        TTF_OpenFont("arial.ttf", 26);

    SDL_Color white = { 255,255,255 };

    SDL_Surface* surface =
        TTF_RenderText_Blended(
            font,
            "Hello SDL_ttf",
            white
        );

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    SDL_FreeSurface(surface);

    SDL_Rect rect = { 100,100,0,0 };

    SDL_QueryTexture(
        texture,
        NULL,
        NULL,
        &rect.w,
        &rect.h
    );

    bool running = true;
    SDL_Event e;

    while (running) {

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                running = false;
        }

        SDL_RenderClear(renderer);

        SDL_RenderCopy(
            renderer,
            texture,
            NULL,
            &rect
        );

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);

    TTF_Quit();
    SDL_Quit();

    return 0;
}