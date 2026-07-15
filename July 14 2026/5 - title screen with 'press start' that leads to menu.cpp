// Program with title screen with flashing 'PRESS START' attract and image, that leads to a menu with 3 different selections.

// NOTE: THIS CURRENTLY HAS A TEXTURE MEMORY LEAK AND WILL CRASH AFTER ABOUT ONE MINUTE IF YOU IDLE ON THE TITLE SCREEN.

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

// Relates to the flashing 'PRESS START' part
const Uint32 BLINK_INTERVAL_MS = 500;
const char* FONT_PATH = "(some ttf fonts/pixeltest-best.ttf";
const int FONT_SIZE = 32;
const char* MESSAGE = "PRESS START";

// Screen system for the menu logic, and screen changes
enum GameState {
    TITLE,
    MENU,
    SCREEN_ONE,
    SCREEN_TWO,
    SCREEN_THREE
};

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) == 0) {
        SDL_Log("IMG_Init failed: %s", IMG_GetError());
    }

    if (TTF_Init() != 0) {
        SDL_Log("TTF_Init failed: %s", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow(
        "Press Start Title Screen Demo With Segue to Menu",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        SDL_Log("CreateWindow failed: %s", SDL_GetError());
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    if (!renderer) {
        SDL_Log("CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont(FONT_PATH, FONT_SIZE);
    if (!font) {
        SDL_Log("TTF_OpenFont failed: %s (did you set FONT_PATH?)", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    bool running = true;
    bool textVisible = true;

    bool onTitleScreen = true;
    bool onTitleScreenMenu = false;

    // These four lines relate to the flashing 'PRESS START' screen
    Uint32 lastToggleTime = SDL_GetTicks();
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Blended(font, MESSAGE, white);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    GameState currentState = TITLE;

    // Three menu buttons (scaled to fit the 640x480 resolution)
    SDL_Rect button1 = { 80, 160, 120, 80 };
    SDL_Rect button2 = { 260, 160, 120, 80 };
    SDL_Rect button3 = { 440, 160, 120, 80 };

    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            switch (currentState) {
            case TITLE:
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;          // ESCAPE quits the whole program here
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    currentState = MENU;
                }
                break;

            case MENU:
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    running = false;          // ESCAPE quits from the menu too, say
                }
                else if (event.type == SDL_MOUSEBUTTONDOWN) {
                    int mouseX = event.button.x;
                    int mouseY = event.button.y;

                    if (mouseX >= button1.x && mouseX <= button1.x + button1.w &&
                        mouseY >= button1.y && mouseY <= button1.y + button1.h)
                        currentState = SCREEN_ONE;
                    else if (mouseX >= button2.x && mouseX <= button2.x + button2.w &&
                        mouseY >= button2.y && mouseY <= button2.y + button2.h)
                        currentState = SCREEN_TWO;
                    else if (mouseX >= button3.x && mouseX <= button3.x + button3.w &&
                        mouseY >= button3.y && mouseY <= button3.y + button3.h)
                        currentState = SCREEN_THREE;
                }
                break;

            case SCREEN_ONE:
            case SCREEN_TWO:
            case SCREEN_THREE:
                // Same key, different meaning: ESCAPE here goes back to the menu
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
                    currentState = MENU;
                }
                break;
            }
        }

        if (currentState == TITLE) {
            // Render the text to a texture
            SDL_Color white = { 255, 255, 255, 255 };
            SDL_Surface* textSurface = TTF_RenderText_Blended(font, MESSAGE, white);
            SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
            int textWidth = textSurface->w;
            int textHeight = textSurface->h;
            SDL_FreeSurface(textSurface);

            SDL_Rect textRect;
            textRect.x = (WINDOW_WIDTH - textWidth) / 2;
            textRect.y = (WINDOW_HEIGHT - textHeight) / 2;
            textRect.w = textWidth;
            textRect.h = textHeight;

            // The next 12 or so lines relate to getting the flashing 'PRESS START' to render.
            Uint32 currentTime = SDL_GetTicks();
            if (currentTime - lastToggleTime >= BLINK_INTERVAL_MS) {
                textVisible = !textVisible;
                lastToggleTime = currentTime;
            }

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Image stuff
            SDL_Surface* imageSurface = IMG_Load("titlescreen-image.png");

            if (!imageSurface)
            {
                SDL_Log("%s", IMG_GetError());
                return 1;
            }

            SDL_Texture* imageTexture = SDL_CreateTextureFromSurface(renderer, imageSurface);

            SDL_FreeSurface(imageSurface);

            SDL_RenderCopy(renderer, imageTexture, NULL, NULL);

            // More flashing 'PRESS START' stuff
            if (textVisible) {
                SDL_RenderCopy(renderer, textTexture, nullptr, &textRect);
            }
        }

        if (currentState == MENU) {
            // Black background
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Draw three gray buttons
            SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);

            SDL_RenderFillRect(renderer, &button1);
            SDL_RenderFillRect(renderer, &button2);
            SDL_RenderFillRect(renderer, &button3);
        }

        if (currentState == SCREEN_ONE) {
            // Red screen
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderClear(renderer);
        }

        if (currentState == SCREEN_TWO) {
            // Green screen
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderClear(renderer);
        }

        if (currentState == SCREEN_THREE) {
            // Blue screen
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderClear(renderer);
        }

        SDL_RenderPresent(renderer);
    }


    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();

    return 0;
}