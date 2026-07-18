#include <SDL.h>
#include <vector>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int PLAYER_SPEED = 4;

struct Room
{
    std::vector<SDL_Rect> walls;

    SDL_Rect eastDoor{};
    SDL_Rect westDoor{};

    bool hasEastDoor = false;
    bool hasWestDoor = false;

    int eastRoom = -1;
    int westRoom = -1;
};

std::vector<Room> rooms;

void BuildRooms()
{
    //-------------------------
    // ROOM 0
    //-------------------------

    Room room0;

    // Top
    room0.walls.push_back({ 0,0,800,20 });

    // Bottom
    room0.walls.push_back({ 0,580,800,20 });

    // Left
    room0.walls.push_back({ 0,20,20,560 });

    // Right split around doorway
    room0.walls.push_back({ 780,20,20,220 });
    room0.walls.push_back({ 780,360,20,220 });

    room0.hasEastDoor = true;
    room0.eastDoor = { 780,240,20,120 };
    room0.eastRoom = 1;

    rooms.push_back(room0);

    //-------------------------
    // ROOM 1
    //-------------------------

    Room room1;

    room1.walls.push_back({ 0,0,800,20 });
    room1.walls.push_back({ 0,580,800,20 });

    // Left split around doorway
    room1.walls.push_back({ 0,20,20,220 });
    room1.walls.push_back({ 0,360,20,220 });

    room1.walls.push_back({ 780,20,20,560 });

    // Interior wall
    room1.walls.push_back({ 250,200,300,20 });

    room1.hasWestDoor = true;
    room1.westDoor = { 0,240,20,120 };
    room1.westRoom = 0;

    rooms.push_back(room1);
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "Two Room Prototype",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        0);

    SDL_Renderer* renderer =
        SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    BuildRooms();

    int currentRoom = 0;

    SDL_Rect player =
    {
        60,
        260,
        32,
        32
    };

    bool quit = false;

    while (!quit)
    {
        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                quit = true;
        }

        int oldX = player.x;
        int oldY = player.y;

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_LEFT])
            player.x -= PLAYER_SPEED;

        if (keys[SDL_SCANCODE_RIGHT])
            player.x += PLAYER_SPEED;

        if (keys[SDL_SCANCODE_UP])
            player.y -= PLAYER_SPEED;

        if (keys[SDL_SCANCODE_DOWN])
            player.y += PLAYER_SPEED;

        Room& room = rooms[currentRoom];

        // Wall collision
        for (const SDL_Rect& wall : room.walls)
        {
            if (SDL_HasIntersection(&player, &wall))
            {
                player.x = oldX;
                player.y = oldY;
                break;
            }
        }

        // East doorway
        if (room.hasEastDoor &&
            SDL_HasIntersection(&player, &room.eastDoor))
        {
            currentRoom = room.eastRoom;

            player.x = 30;
            player.y = 260;
        }

        // West doorway
        if (room.hasWestDoor &&
            SDL_HasIntersection(&player, &room.westDoor))
        {
            currentRoom = room.westRoom;

            player.x = 740;
            player.y = 260;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        room = rooms[currentRoom];

        // Draw walls
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        for (const SDL_Rect& wall : room.walls)
            SDL_RenderFillRect(renderer, &wall);

        // Draw player
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &player);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}