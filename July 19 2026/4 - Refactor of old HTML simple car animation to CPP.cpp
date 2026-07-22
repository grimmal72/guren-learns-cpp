#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
using namespace std;

const int CANVAS_WIDTH = 900;
const int CANVAS_HEIGHT = 550;


int main(int argc, char* argv[]) {

    SDL_Init(SDL_INIT_VIDEO);

    int imgFlags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initializedFlags = IMG_Init(imgFlags);
    if ((initializedFlags & imgFlags) != imgFlags) {
        printf("IMG_Error: %s\n", IMG_GetError());
    }

    SDL_Window* window = SDL_CreateWindow("Car Moving Animation",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        CANVAS_WIDTH, CANVAS_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Surface* carSurface = IMG_Load("car.png");
    SDL_Surface* backgroundSurface = IMG_Load("greenery.jpg");

    if (!(carSurface && backgroundSurface))
    {
        SDL_Log("%s", IMG_GetError());
        return 1;
    }

    // Convert surface to texture
    SDL_Texture* carTexture = SDL_CreateTextureFromSurface(renderer, carSurface);
    SDL_Texture* backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);


    // Surface no longer needed
    SDL_FreeSurface(carSurface);
    SDL_FreeSurface(backgroundSurface);

    SDL_Rect backgroundDestRect = { 0, 0, 900, 550 };
    SDL_Rect carDestRect = { 0, 350, 322, 195 }; // Note that you can change these dimensions and even if it's a squashed rect the car texture will still try to draw in squashed.

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        SDL_RenderClear(renderer);

        if (carDestRect.x <= 580) {
            carDestRect.x += 5;
        }

        SDL_RenderCopy(renderer, backgroundTexture, NULL, &backgroundDestRect);
        SDL_RenderCopy(renderer, carTexture, NULL, &carDestRect);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(carTexture);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    SDL_Quit();

    return 0;
}









/* Here is the old JavaScript code to try to translate to C++:

const canvas = document.getElementById("scene");
const context = canvas.getContext("2d");
let img1 = new Image();
let img2 = new Image();

img1.src = 'img/greenery.jpg';
img2.src = 'img/car.png';

canvas.width = 900;
canvas.height = 550;

img2.onload = function () {
    context.clearRect(0, 0, canvas.width, canvas.height);
    
    draw background image
    context.drawImage(img1, 0, 0, canvas.width, canvas.height);

    context.drawImage(img2, xposition, yposition, 200, 160);
    if (xposition > 700) {
        return;
    }
    xposition = xposition + 3;
}

let xposition = 0;
let yposition = 400;



setInterval(img2.onload, 25)

*/
