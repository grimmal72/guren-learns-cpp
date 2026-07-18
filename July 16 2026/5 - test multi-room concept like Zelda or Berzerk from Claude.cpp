#include <SDL.h>
#include <vector>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

struct Item {
    SDL_Rect rect;
    bool collected = false;
};

struct Room {
    SDL_Color bgColor;
    std::vector<Item> items;
    SDL_Rect doorToOtherRoom;
};

int currentRoom = 0;
Room rooms[2];

void InitRooms() {
    rooms[0].bgColor = { 30, 30, 60, 255 };
    rooms[0].items.push_back({ { 300, 200, 20, 20 }, false }); // a pickup
    rooms[0].doorToOtherRoom = { 600, 200, 20, 40 };

    rooms[1].bgColor = { 60, 30, 30, 255 };
    rooms[1].items.push_back({ { 100, 100, 20, 20 }, false });
    rooms[1].doorToOtherRoom = { 0, 200, 20, 40 };
}

void RenderRoom(SDL_Renderer* renderer) {
    Room& room = rooms[currentRoom];

    SDL_SetRenderDrawColor(renderer, room.bgColor.r, room.bgColor.g, room.bgColor.b, 255);
    SDL_RenderClear(renderer);

    // Draw the door
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &room.doorToOtherRoom);

    // Draw items — only if not collected. This is the "appear/disappear" logic.
    SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
    for (const Item& item : room.items) {
        if (!item.collected) {
            SDL_RenderFillRect(renderer, &item.rect);
        }
    }

    SDL_RenderPresent(renderer);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("Room Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    InitRooms();
    RenderRoom(renderer);

    bool running = true;
    SDL_Event e;
    while (running) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) running = false;

            if (e.type == SDL_KEYDOWN) {
                // Room change: press SPACE to walk through the door
                if (e.key.keysym.sym == SDLK_SPACE) {
                    currentRoom = (currentRoom == 0) ? 1 : 0;
                    RenderRoom(renderer);
                }
                // Item pickup: press C to collect the item in this room
                if (e.key.keysym.sym == SDLK_c) {
                    for (Item& item : rooms[currentRoom].items) {
                        item.collected = true;
                    }
                    RenderRoom(renderer);
                }
            }
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}