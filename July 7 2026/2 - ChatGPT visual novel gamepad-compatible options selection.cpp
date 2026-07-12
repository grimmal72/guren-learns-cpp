#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow(
        "Dialogue Menu",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("lazy.ttf", 28);

    SDL_GameController* controller = nullptr;

    if (SDL_NumJoysticks() > 0 &&
        SDL_IsGameController(0))
    {
        controller = SDL_GameControllerOpen(0);
    }

    SDL_Rect options[2];

    // "options" and "text" are both arrays that point to the two boxes that we control.
    // But options defines the size of the boxes themselves.
    // And text is the string text that we'll put in the box later.
    // This goes x, y, w, h
    options[0] = { 150, 200, 500, 60 };
    options[1] = { 150, 290, 500, 60 };

    const char* text[2] =
    {
        "Option 1",
        "Option 2"
    };

    int selected = 0;

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = false;
                break;

            case SDL_CONTROLLERBUTTONDOWN:

                switch (event.cbutton.button)
                {
                case SDL_CONTROLLER_BUTTON_DPAD_UP:

                    if (selected > 0)
                        selected--;

                    break;

                case SDL_CONTROLLER_BUTTON_DPAD_DOWN:

                    if (selected < 1)
                        selected++;

                    break;

                case SDL_CONTROLLER_BUTTON_A:

                    std::cout
                        << "You selected: "
                        << text[selected]
                        << std::endl;

                    break;
                }

                break;
            }
        }

        //-------------------------------------
        // Draw
        //-------------------------------------

        SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
        SDL_RenderClear(renderer);

        for (int i = 0; i < 2; i++)
        {
            // Selected option = yellow
            if (i == selected)
                SDL_SetRenderDrawColor(renderer, 230, 210, 0, 255);
            else
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);

            SDL_RenderFillRect(renderer, &options[i]);

            //---------------------------------
            // Render text
            //---------------------------------

            SDL_Color black = { 0,0,0,255 };

            SDL_Surface* surface =
                TTF_RenderText_Blended(
                    font,
                    text[i],
                    black);

            SDL_Texture* texture =
                SDL_CreateTextureFromSurface(
                    renderer,
                    surface);

            SDL_Rect dest;

            dest.w = surface->w;
            dest.h = surface->h;

            // Center text inside rectangle
            dest.x = options[i].x +
                (options[i].w - dest.w) / 2;

            dest.y = options[i].y +
                (options[i].h - dest.h) / 2;

            SDL_FreeSurface(surface);

            SDL_RenderCopy(
                renderer,
                texture,
                NULL,
                &dest);

            SDL_DestroyTexture(texture);
        }

        SDL_RenderPresent(renderer);
    }

    if (controller)
        SDL_GameControllerClose(controller);

    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}