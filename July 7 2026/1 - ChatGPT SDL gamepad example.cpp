#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

    SDL_Window* window = SDL_CreateWindow(
        "Gamepad Example",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800,
        600,
        SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Open the first controller found
    SDL_GameController* controller = nullptr;

    if (SDL_NumJoysticks() > 0)
    {
        if (SDL_IsGameController(0))
        {
            controller = SDL_GameControllerOpen(0);

            if (controller)
                std::cout << "Controller connected!\n";
        }
    }

    SDL_Rect player = { 375, 275, 50, 50 };

    Uint8 r = 255;
    Uint8 g = 0;
    Uint8 b = 0;

    bool running = true;
    SDL_Event event;

    const int speed = 5;

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
                case SDL_CONTROLLER_BUTTON_A:
                    r = 255; g = 0; b = 0;
                    break;

                case SDL_CONTROLLER_BUTTON_B:
                    r = 0; g = 255; b = 0;
                    break;

                case SDL_CONTROLLER_BUTTON_X:
                    r = 0; g = 0; b = 255;
                    break;

                case SDL_CONTROLLER_BUTTON_Y:
                    r = 255; g = 255; b = 0;
                    break;
                }

                break;
            }
        }

        // Analog stick movement
        if (controller)
        {
            Sint16 x =
                SDL_GameControllerGetAxis(
                    controller,
                    SDL_CONTROLLER_AXIS_LEFTX);

            Sint16 y =
                SDL_GameControllerGetAxis(
                    controller,
                    SDL_CONTROLLER_AXIS_LEFTY);

            const int DEADZONE = 8000;

            if (x > DEADZONE)
                player.x += speed;

            if (x < -DEADZONE)
                player.x -= speed;

            if (y > DEADZONE)
                player.y += speed;

            if (y < -DEADZONE)
                player.y -= speed;

            // D-pad movement
            if (SDL_GameControllerGetButton(
                controller,
                SDL_CONTROLLER_BUTTON_DPAD_LEFT))
                player.x -= speed;

            if (SDL_GameControllerGetButton(
                controller,
                SDL_CONTROLLER_BUTTON_DPAD_RIGHT))
                player.x += speed;

            if (SDL_GameControllerGetButton(
                controller,
                SDL_CONTROLLER_BUTTON_DPAD_UP))
                player.y -= speed;

            if (SDL_GameControllerGetButton(
                controller,
                SDL_CONTROLLER_BUTTON_DPAD_DOWN))
                player.y += speed;
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderFillRect(renderer, &player);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    if (controller)
        SDL_GameControllerClose(controller);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}