#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow(
        "Typewriter Effect",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0);

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    TTF_Font* font = TTF_OpenFont("(some ttf fonts/arial.ttf", 28);

    std::string fullText =
        "This is an example paragraph being written out one\n"
        "character at a time. SDL_ttf renders only\n"
        "the visible portion, producing a classic RPG\n"
        "or visual novel typewriter effect.";

    size_t visibleCharacters = 0;

    Uint32 lastCharacterTime = SDL_GetTicks();
    const Uint32 characterDelay = 40; // milliseconds per character

    SDL_Texture* textTexture = nullptr;
    SDL_Rect textRect;

    bool running = true;

    while (running)
    {
        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
                running = false;
        }

        // Reveal another character every 40 ms
        if (visibleCharacters < fullText.size())
        {
            Uint32 now = SDL_GetTicks();

            if (now - lastCharacterTime >= characterDelay)
            {
                visibleCharacters++;
                lastCharacterTime = now;

                // Destroy previous texture
                if (textTexture)
                    SDL_DestroyTexture(textTexture);

                std::string visibleText =
                    fullText.substr(0, visibleCharacters);

                SDL_Color color = { 0, 0, 0, 255 };

                SDL_Surface* surface =
                    TTF_RenderUTF8_Blended_Wrapped(
                        font,
                        visibleText.c_str(),
                        color,
                        700);   // Wrap after 700 pixels

                textTexture =
                    SDL_CreateTextureFromSurface(renderer, surface);

                textRect.x = 50;
                textRect.y = 50;
                textRect.w = surface->w;
                textRect.h = surface->h;

                SDL_FreeSurface(surface);
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        if (textTexture)
            SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}