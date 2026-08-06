// room_system.cpp
//
// Simplest possible "room" navigation system.
// SDL2 is used ONLY to open a tiny window and read arrow-key events
// (SDL needs a window with focus to receive keyboard input).
// All actual output ("You are now in Room X") goes to the console.
//
// Controls: Arrow keys move between rooms. Esc or closing the window quits.

#include <SDL.h>
#include <iostream>
#include <string>

// ---- 1. Name every room ----
enum Room {
    ROOM1, ROOM2, ROOM3, ROOM4,
    ROOM5, ROOM6, ROOM7, ROOM8,
    NUMBERED_ROOMS,
    NO_ROOM = -1
};

// Human-readable names array for console output
std::string roomNames[NUMBERED_ROOMS] = {
    "Room 1", "Room 2", "Room 3", "Room 4",
    "Room 5", "Room 6", "Room 7", "Room 8"
};

// ---- 2. Each room lists what's through each of its 4 exits ----
struct RoomExits {
    int north;
    int south;
    int east;
    int west;
};

// ---- 3. The map: one row per room, filled in by hand ----
RoomExits worldMap[NUMBERED_ROOMS] = {
    /* ROOM1 */ { NO_ROOM, NO_ROOM, ROOM2,   NO_ROOM },
    /* ROOM2 */ { NO_ROOM, ROOM6,   ROOM3,   ROOM1   },
    /* ROOM3 */ { NO_ROOM, ROOM7,   ROOM4,   ROOM2   },
    /* ROOM4 */ { NO_ROOM, ROOM8,   NO_ROOM, ROOM3   },
    /* ROOM5 */ { NO_ROOM, NO_ROOM, ROOM6,   NO_ROOM },
    /* ROOM6 */ { ROOM2,   NO_ROOM, ROOM7,   ROOM5   },
    /* ROOM7 */ { ROOM3,   NO_ROOM, ROOM8,   ROOM6   },
    /* ROOM8 */ { ROOM4,   NO_ROOM, NO_ROOM, ROOM7   },
};

Room currentRoom = ROOM1;

// ---- 4. Moving is one function, not one switch-case per room ----
void tryMove(const std::string& direction) {
    int next = NO_ROOM;
    RoomExits& exits = worldMap[currentRoom];

    if (direction == "north")
    { next = exits.north; }

    else if (direction == "south")
    { next = exits.south; }

    else if (direction == "east")
    { next = exits.east; }

    else if (direction == "west")
    { next = exits.west; }

    if (next != NO_ROOM) {
        currentRoom = (Room)next;
        std::cout << "You went " << direction << ". You are now in "
            << roomNames[currentRoom] << ".\n";
    }
    else {
        std::cout << "You can't go " << direction << " from "
            << roomNames[currentRoom] << ".\n";
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        return 1;
    }

    // SDL only delivers keyboard events to a focused window, so we need
    // one on screen even though we don't draw anything into it.
    SDL_Window* window = SDL_CreateWindow(
        "Room Navigator (use arrow keys, Esc to quit)",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        400, 200,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    std::cout << "You are in " << roomNames[currentRoom] << ".\n";
    std::cout << "Use arrow keys to move. Esc to quit.\n\n";

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                case SDLK_UP:    tryMove("north"); break;
                case SDLK_DOWN:  tryMove("south"); break;
                case SDLK_RIGHT: tryMove("east");  break;
                case SDLK_LEFT:  tryMove("west");  break;
                case SDLK_ESCAPE: running = false;  break;
                }
            }
        }
        SDL_Delay(16); // don't busy-loop the CPU
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}