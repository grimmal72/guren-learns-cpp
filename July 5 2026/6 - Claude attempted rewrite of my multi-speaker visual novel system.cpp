// Test dialogue sequence with textbox with differing speaker names.cpp

// The big deal here might be setting a variable like currentSpeakingCharacter, and then 
// whatever currentSpeakingCharacter is, the little spot above the textbox says their name.
// Very simple, theoretically. Right now I won't think about changing the character sprite along with this.

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include <iostream>
#include <string>

// Window settings
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

enum CharactersThatMaySpeak {
    AERITH,
    YOU
};

CharactersThatMaySpeak currentSpeakingCharacter = AERITH;

// First line of text will be by Aerith, so that's why it defaults to this.
std::string speakerNameText = "Aerith";

// Draws the little nameplate above the textbox showing who's currently speaking.
// Builds and destroys its own surface/texture each call since this text changes
// far less often than every frame, but it's cheap enough not to bother caching.
void whoIsSpeaking(int dialogue, SDL_Color black, TTF_Font* font, SDL_Renderer* renderer) {

    switch (dialogue) {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        currentSpeakingCharacter = AERITH;
        break;
    case 10:
        currentSpeakingCharacter = YOU;
        break;
    }

    if (currentSpeakingCharacter == AERITH) {
        speakerNameText = "Aerith";
    }
    else if (currentSpeakingCharacter == YOU) {
        speakerNameText = "You";
    }

    SDL_Surface* speakerNameSurface =
        TTF_RenderText_Blended(
            font,
            speakerNameText.c_str(),
            black
        );

    SDL_Texture* speakerNameTexture =
        SDL_CreateTextureFromSurface(
            renderer,
            speakerNameSurface
        );

    SDL_FreeSurface(speakerNameSurface);

    SDL_Rect speakerNameRect = { 100, 450, 0, 0 };

    SDL_QueryTexture(
        speakerNameTexture,
        NULL,
        NULL,
        &speakerNameRect.w,
        &speakerNameRect.h
    );

    SDL_RenderCopy(
        renderer,
        speakerNameTexture,
        NULL,
        &speakerNameRect
    );

    SDL_DestroyTexture(speakerNameTexture);
};


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
        "Talking to woman (visual novel system)",
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

    // --- Main dialogue textbox text ---
    std::string text = "Oh, hey, it's you.";

    SDL_Surface* textBodySurface =
        TTF_RenderText_Blended(
            font,
            text.c_str(),
            black
        );

    SDL_Texture* textBodyTexture =
        SDL_CreateTextureFromSurface(
            renderer,
            textBodySurface
        );

    SDL_FreeSurface(textBodySurface);

    SDL_Rect textBodyRect = { 100, 500, 0, 0 };

    SDL_QueryTexture(
        textBodyTexture,
        NULL,
        NULL,
        &textBodyRect.w,
        &textBodyRect.h
    );

    // --- Character portrait image (loaded once, not every frame) ---
    SDL_Surface* portraitSurface = IMG_Load("image3.jpg");
    if (!portraitSurface)
    {
        SDL_Log("%s", IMG_GetError());
        return 1;
    }

    SDL_Texture* portraitTexture = SDL_CreateTextureFromSurface(renderer, portraitSurface);
    SDL_FreeSurface(portraitSurface);

    SDL_Rect portraitImageRect = { 100, 100, 500, 300 };

    int dialogue = 0;

    bool running = true;
    SDL_Event event;
    SDL_Rect textboxBackground;
    SDL_Rect portraitBackground;

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
                SDL_DestroyTexture(textBodyTexture);

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

                case 10:
                    text = "*sigh* Come on, Aerith.";
                }

                // Make a new surface
                textBodySurface =
                    TTF_RenderText_Blended(
                        font,
                        text.c_str(),
                        black
                    );

                // Turn it into a texture
                textBodyTexture =
                    SDL_CreateTextureFromSurface(
                        renderer,
                        textBodySurface
                    );

                SDL_FreeSurface(textBodySurface);

                // Update text size
                SDL_QueryTexture(
                    textBodyTexture,
                    NULL,
                    NULL,
                    &textBodyRect.w,
                    &textBodyRect.h
                );
            }
        }

        // White background to renderer
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Grey textbox
        textboxBackground = {
            (SCREEN_WIDTH - 700) / 2,   // x
            (SCREEN_HEIGHT - 100),      // y
            700,                        // width
            50                          // height
        };
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 10);
        SDL_RenderFillRect(renderer, &textboxBackground);

        // Big character sprite portrait box
        portraitBackground = {
            (SCREEN_WIDTH - 700) / 2,   // x
            (SCREEN_HEIGHT - 550),      // y
            700,                        // width
            400                         // height
        };
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 10);
        SDL_RenderFillRect(renderer, &portraitBackground);

        // Draw dialogue text
        SDL_RenderCopy(
            renderer,
            textBodyTexture,
            NULL,
            &textBodyRect
        );

        // Display current speaking character name
        whoIsSpeaking(dialogue, black, font, renderer);

        // Draw character portrait
        SDL_RenderCopy(renderer, portraitTexture, NULL, &portraitImageRect);

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyTexture(textBodyTexture);
    SDL_DestroyTexture(portraitTexture);
    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}