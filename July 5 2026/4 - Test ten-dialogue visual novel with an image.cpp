#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

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

    // Initialize SDL_image
    if (IMG_Init(IMG_INIT_JPG) < 0)
    {
        std::cout << "SDL_image could not initialize.\n";
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
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
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
    TTF_Font* font = TTF_OpenFont("(some ttf fonts/arial.ttf", 30);

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

    std::string text = "Oh, hey, it's you.";

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

    SDL_Rect textRect = { 100, 500, 0, 0 };

    SDL_QueryTexture(
        texture,
        NULL,
        NULL,
        &textRect.w,
        &textRect.h
    );

    int dialogue = 0;
    bool running = true;
    SDL_Event event;
    SDL_Rect textbox;
    SDL_Rect portraitBox;

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

                dialogue++;

                // Destroy old texture
                SDL_DestroyTexture(texture);

                // Change what the text says
                switch (dialogue)
                {
                case 1:
                    text = "Where have you been?";
                    break;

                case 2:
                    text = "It's been a while!";
                    break;

                case 3:
                    text = "I was worried about you!";
                    break;

                case 4:
                    text = "Don't make me worry about you!";
                    break;

                case 5:
                    text = "You silly!";
                    break;

                case 6:
                    text = "*Bonks your head*";
                    break;

                case 7:
                    text = "Don't be silly!";
                    break;

                case 8:
                    text = "Geez..";
                    break;

                case 9:
                    text = "It's OK, though. I forgive you.";
                    break;
                }

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

        // White background to renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Grey textbox
        textbox = {
            (SCREEN_WIDTH - 700) / 2,   // x
            (SCREEN_HEIGHT - 100),  // y
            700,                        // width
            50                         // height
        };
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 10);
        SDL_RenderFillRect(renderer, &textbox);

        // Big character sprite portrait box
        portraitBox = {
            (SCREEN_WIDTH - 700) / 2,   // x
            (SCREEN_HEIGHT - 550),  // y
            700,                        // width
            400                         // height
        };
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 10);
        SDL_RenderFillRect(renderer, &portraitBox);

        // Draw text
        SDL_RenderCopy(
            renderer,
            texture,
            NULL,
            &textRect
        );

        // Load PNG into a surface
        SDL_Surface* surface = IMG_Load("image3.jpg");
        if (!surface)
        {
            SDL_Log("%s", IMG_GetError());
            return 1;
        }
        // Convert surface to texture
        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        // Surface no longer needed
        SDL_FreeSurface(surface);
        SDL_Rect dest = { 100, 100, 500, 300 };
        SDL_RenderCopy(renderer, texture, NULL, &dest);

        // The approximately 12 lines of code above this are for rendering the character image.


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