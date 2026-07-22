#include <SDL.h>
#include <iostream>
#include <vector>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int WORLD_WIDTH = 32;
const int WORLD_HEIGHT = 24;

const int SCALE = 20;


//--------------------------------------------------
// Create X shape
//--------------------------------------------------

std::vector<SDL_Point> CreateX()
{
    std::vector<SDL_Point> points;

    for (int i = 0; i < 5; i++)
    {
        // top-left to bottom-right
        points.push_back({ 14 + i, 10 + i });

        // top-right to bottom-left
        points.push_back({ 18 - i, 10 + i });
    }

    return points;
}


//--------------------------------------------------
// Check if mouse is inside hotspot
//--------------------------------------------------

bool IsClicked(
    int mouseX,
    int mouseY,
    const std::vector<std::vector<int>>& mask,
    int id)
{
    if (mouseX < 0 || mouseX >= WORLD_WIDTH ||
        mouseY < 0 || mouseY >= WORLD_HEIGHT)
    {
        return false;
    }

    return mask[mouseY][mouseX] == id;
}


int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);


    SDL_Window* window = SDL_CreateWindow(
        "Pixel Hotspot",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN);


    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED);


    //--------------------------------------------------
    // Create object
    //--------------------------------------------------

    std::vector<SDL_Point> xShape = CreateX();


    const int X_OBJECT = 1;


    //--------------------------------------------------
    // Create invisible collision map
    //--------------------------------------------------

    std::vector<std::vector<int>> hotspot(
        WORLD_HEIGHT,
        std::vector<int>(WORLD_WIDTH, 0)
    );


    for (SDL_Point p : xShape)
    {
        hotspot[p.y][p.x] = X_OBJECT;
    }



    //--------------------------------------------------

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


            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int worldX = event.button.x / SCALE;
                int worldY = event.button.y / SCALE;


                if (IsClicked(
                    worldX,
                    worldY,
                    hotspot,
                    X_OBJECT))
                {
                    std::cout
                        << "You clicked the X!"
                        << std::endl;
                }
            }
        }


        //--------------------------------------------------
        // Draw
        //--------------------------------------------------

        SDL_SetRenderDrawColor(
            renderer,
            255,
            255,
            255,
            255);

        SDL_RenderClear(renderer);


        SDL_SetRenderDrawColor(
            renderer,
            255,
            0,
            0,
            255);


        for (SDL_Point p : xShape)
        {
            SDL_Rect pixel;

            pixel.x = p.x * SCALE;
            pixel.y = p.y * SCALE;
            pixel.w = SCALE;
            pixel.h = SCALE;

            SDL_RenderFillRect(renderer, &pixel);
        }


        SDL_RenderPresent(renderer);
    }


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}