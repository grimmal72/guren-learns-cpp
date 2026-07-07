#include <SDL.h>
#include <SDL_ttf.h>

#include <iostream>
#include <string>

// Window settings
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "SDL could not initialize.\n";
        return 1;
    }

    // Initialize SDL_ttf
    if (TTF_Init() == -1)
    {
        std::cout << "SDL_ttf could not initialize.\n";
        SDL_Quit();
        return 1;
    }

    // Create window
    SDL_Window* window = SDL_CreateWindow(
        "Click Counter",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window)
    {
        std::cout << "Window creation failed.\n";
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!renderer)
    {
        std::cout << "Renderer creation failed.\n";
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // Load font
    TTF_Font* font = TTF_OpenFont("arial.ttf", 102);

    if (!font)
    {
        std::cout << "Couldn't load font.\n";
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Color black = { 0, 0, 0, 255 };

    // Counter
    int counter = 0;

    // Create the initial text texture ("0")
    std::string text = std::to_string(counter);

    SDL_Surface* surface =
        TTF_RenderText_Blended(
            font,
            text.c_str(),
            black
        );

    SDL_Texture* texture =
        SDL_CreateTextureFromSurface(
            renderer,
            surface
        );

    SDL_FreeSurface(surface);

    SDL_Rect textRect = { 20, 20, 0, 0 };

    SDL_QueryTexture(
        texture,
        NULL,
        NULL,
        &textRect.w,
        &textRect.h
    );

    // Red button in the center
    SDL_Rect button =
    {
        (SCREEN_WIDTH - 200) / 2,
        (SCREEN_HEIGHT - 100) / 2,
        200,
        100
    };

    bool running = true;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    running = false;
                }
            }

            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                SDL_Point mousePoint =
                {
                    event.button.x,
                    event.button.y
                };

                if (SDL_PointInRect(&mousePoint, &button))
                {
                    // Increase counter
                    counter++;

                    // Destroy old texture
                    SDL_DestroyTexture(texture);

                    // Convert number to text
                    text = std::to_string(counter);

                    // Make a new surface
                    surface =
                        TTF_RenderText_Blended(
                            font,
                            text.c_str(),
                            black
                        );

                    // Turn it into a texture
                    texture =
                        SDL_CreateTextureFromSurface(
                            renderer,
                            surface
                        );

                    SDL_FreeSurface(surface);

                    // Update text size
                    SDL_QueryTexture(
                        texture,
                        NULL,
                        NULL,
                        &textRect.w,
                        &textRect.h
                    );
                }
            }
        }

        // White background
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Red button
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &button);

        // Draw counter
        SDL_RenderCopy(
            renderer,
            texture,
            NULL,
            &textRect
        );

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}