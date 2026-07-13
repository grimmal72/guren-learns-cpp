#include <SDL.h>
#include <iostream>

// Window settings
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Different program states
enum GameState {
    MENU,
    SCREEN_ONE,
    SCREEN_TWO,
    SCREEN_THREE
};

int main(int argc, char* argv[])
{
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize\n";
        return 1;
    }

    // Create centered window
    SDL_Window* window = SDL_CreateWindow(
        "SDL2 Menu Example",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cout << "Window creation failed\n";
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    if (!renderer) {
        std::cout << "Renderer creation failed\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    GameState currentState = MENU;

    // Three menu buttons
    SDL_Rect button1 = { 100, 200, 150, 100 };
    SDL_Rect button2 = { 325, 200, 150, 100 };
    SDL_Rect button3 = { 550, 200, 150, 100 };

    while (running)
    {
        // -------- Event Handling --------
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }

            // Keyboard input
            if (event.type == SDL_KEYDOWN)
            {
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    if (currentState == MENU)
                        running = false;
                    else
                        currentState = MENU;
                }
            }

            // Mouse click handling
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int mouseX = event.button.x;
                int mouseY = event.button.y;

                // Only check buttons when on menu
                if (currentState == MENU)
                {
                    // Check button 1
                    if (mouseX >= button1.x &&
                        mouseX <= button1.x + button1.w &&
                        mouseY >= button1.y &&
                        mouseY <= button1.y + button1.h)
                    {
                        currentState = SCREEN_ONE;
                    }

                    // Check button 2
                    else if (mouseX >= button2.x &&
                        mouseX <= button2.x + button2.w &&
                        mouseY >= button2.y &&
                        mouseY <= button2.y + button2.h)
                    {
                        currentState = SCREEN_TWO;
                    }

                    // Check button 3
                    else if (mouseX >= button3.x &&
                        mouseX <= button3.x + button3.w &&
                        mouseY >= button3.y &&
                        mouseY <= button3.y + button3.h)
                    {
                        currentState = SCREEN_THREE;
                    }
                }
            }
        }

        // -------- Rendering --------
        switch (currentState)
        {
        case MENU:
        {
            // Black background
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);

            // Draw three gray buttons
            SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);

            SDL_RenderFillRect(renderer, &button1);
            SDL_RenderFillRect(renderer, &button2);
            SDL_RenderFillRect(renderer, &button3);

            break;
        }

        case SCREEN_ONE:
        {
            // Red screen
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
            SDL_RenderClear(renderer);
            break;
        }

        case SCREEN_TWO:
        {
            // Green screen
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
            SDL_RenderClear(renderer);
            break;
        }

        case SCREEN_THREE:
        {
            // Blue screen
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderClear(renderer);
            break;
        }
        }

        SDL_RenderPresent(renderer);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}