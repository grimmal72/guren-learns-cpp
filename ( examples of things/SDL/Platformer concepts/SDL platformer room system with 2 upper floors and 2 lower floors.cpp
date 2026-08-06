#include <SDL.h>
#include <vector>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int FALL_THROUGH_THE_GROUND_EDGES_PREVENTION = 20;

const float GRAVITY = 0.5f;
const float MOVE_SPEED = 5.0f;
const float JUMP_SPEED = -12.0f;

struct Platform
{
    SDL_Rect rect;
};

struct Exit
{
    SDL_Rect trigger;
    int targetRoom;
    float spawnX;
    float spawnY;
    bool keepX = false;
    bool keepY = false;
};

struct Room
{
    SDL_Color background = { 100, 180, 255, 255 };
    std::vector<Platform> platforms = {};
    std::vector<Exit> exits = {};
};

struct Player
{
    float x = 100;
    float y = 100;

    float vx = 0;
    float vy = 0;

    int w = 32;
    int h = 48;

    bool onGround = false;
};

bool Intersects(const Player& player, const SDL_Rect& rect)
{
    SDL_Rect p =
    {
        (int)player.x,
        (int)player.y,
        player.w,
        player.h
    };

    return SDL_HasIntersection(&p, &rect);
}

// Checks the given room's exits against the player. If one triggers,
// moves the player into the target room and returns true.
bool TryExits(Room& room, Player& player, int& currentRoom)
{
    for (const auto& exit : room.exits)
    {
        if (Intersects(player, exit.trigger))
        {
            currentRoom = exit.targetRoom;

            if (!exit.keepX)
                player.x = exit.spawnX;

            if (!exit.keepY)
                player.y = exit.spawnY;

            player.vx = 0;
            player.vy = 0;
            player.onGround = false;
            return true;
        }
    }

    return false;
}

int main(int argc, char* argv[])
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
        "SDL Four Rooms",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Player player;

    //----------------------------------

    std::vector<Room> rooms(4);

    const int DOOR_THICK = 30;

    // How far in from the edge a player spawns when entering a room from
    // its far side. Needs to clear DOOR_THICK + player.w by a comfortable
    // margin, or the spawn point overlaps that room's own exit trigger
    // and you get bounced straight back out the same frame you arrive.
    const int DOOR_SPAWN_INSET = 100;

    //---------------- Room 0: top-left ----------------
    rooms[0].background = { 100, 180, 255, 255 };
    rooms[0].platforms =
    {
        {{-FALL_THROUGH_THE_GROUND_EDGES_PREVENTION, 560, 300, 40}}, // floor left half (x: -20..280)
        {{380, 560, 440 + FALL_THROUGH_THE_GROUND_EDGES_PREVENTION, 40}}, // floor right half (x: 380..820) -- gap 280..380
        {{100, 430, 180, 20}},
        {{560, 340, 150, 20}}
    };
    rooms[0].exits =
    {
        // Right edge -> room 1. keepY = true so your height carries over
        // instead of snapping to a fixed door height.
        { { SCREEN_WIDTH - DOOR_THICK, 0, DOOR_THICK, SCREEN_HEIGHT }, 1, 20.0f, 0.0f, false, true },
        // Hole in the floor -> room 2 (a tall zone under the gap catches the player as they fall)
        { { 280, 560, 100, 200 }, 2, 310.0f, 220.0f }
    };

    //---------------- Room 1: top-right ----------------
    rooms[1].background = { 150, 120, 200, 255 };
    rooms[1].platforms =
    {
        {{-FALL_THROUGH_THE_GROUND_EDGES_PREVENTION, 560, 800 + (FALL_THROUGH_THE_GROUND_EDGES_PREVENTION * 2), 40}},
        {{550, 420, 150, 20}},
        {{200, 320, 150, 20}}
    };
    rooms[1].exits =
    {
        // Left edge -> room 0
        { { -DOOR_THICK, 0, DOOR_THICK, SCREEN_HEIGHT }, 0, (float)(SCREEN_WIDTH - DOOR_SPAWN_INSET), 0.0f, false, true }
    };

    //---------------- Room 2: bottom-left ----------------
    rooms[2].background = { 40, 40, 90, 255 };
    rooms[2].platforms =
    {
        {{-FALL_THROUGH_THE_GROUND_EDGES_PREVENTION, 560, 800 + (FALL_THROUGH_THE_GROUND_EDGES_PREVENTION * 2), 40}}, // ground
        {{-FALL_THROUGH_THE_GROUND_EDGES_PREVENTION, 0, 300, 40}}, // roof left half
        {{380, 0, 440 + FALL_THROUGH_THE_GROUND_EDGES_PREVENTION, 40}}, // roof right half
        {{150, 470, 150, 20}},
        {{350, 380, 150, 20}},
        {{300, 270, 160, 20}},
        {{250, 170, 150, 20}}
    };
    rooms[2].exits =
    {
        // Right edge -> room 3
        { { SCREEN_WIDTH - DOOR_THICK, 0, DOOR_THICK, SCREEN_HEIGHT }, 3, 20.0f, 0.0f, false, true },
        { { 330, 40, 80, 20 }, 0, 150.0f, 400.0f, true, false  }
    };

    //---------------- Room 3: bottom-right ----------------
    rooms[3].background = { 90, 40, 40, 255 };
    rooms[3].platforms =
    {
        {{-FALL_THROUGH_THE_GROUND_EDGES_PREVENTION, 560, 800 + (FALL_THROUGH_THE_GROUND_EDGES_PREVENTION * 2), 40}},
        {{500, 450, 150, 20}},
        {{250, 350, 150, 20}}
    };
    rooms[3].exits =
    {
        // Left edge -> room 2
        { { -DOOR_THICK, 0, DOOR_THICK, SCREEN_HEIGHT }, 2, (float)(SCREEN_WIDTH - DOOR_SPAWN_INSET), 0.0f, false, true }
    };

    int currentRoom = 0;

    bool running = true;

    while (running)
    {
        SDL_Event e;

        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = false;
        }

        const Uint8* keys = SDL_GetKeyboardState(NULL);

        if (keys[SDL_SCANCODE_ESCAPE])
            running = false;

        player.vx = 0;

        if (keys[SDL_SCANCODE_LEFT])
            player.vx = -MOVE_SPEED;

        if (keys[SDL_SCANCODE_RIGHT])
            player.vx = MOVE_SPEED;

        if (keys[SDL_SCANCODE_SPACE] && player.onGround)
        {
            player.vy = JUMP_SPEED;
            player.onGround = false;
        }

        //----------------------------------
        // Horizontal movement
        //----------------------------------

        {
            Room& room = rooms[currentRoom];

            player.x += player.vx;

            for (const auto& p : room.platforms)
            {
                if (Intersects(player, p.rect))
                {
                    if (player.vx > 0)
                        player.x = p.rect.x - player.w;

                    if (player.vx < 0)
                        player.x = p.rect.x + p.rect.w;
                }
            }
        }

        //----------------------------------
        // Check for room transitions (left/right doorways live here,
        // since they're only reachable via horizontal movement)
        //----------------------------------

        TryExits(rooms[currentRoom], player, currentRoom);

        //----------------------------------
        // Fallback: if we walked off an edge that has no exit wired up,
        // clamp so the player stays flush against the edge instead of
        // disappearing off-screen.
        //----------------------------------

        if (player.x < 0)
            player.x = 0;

        if (player.x > SCREEN_WIDTH - player.w)
            player.x = (float)(SCREEN_WIDTH - player.w);

        //----------------------------------
        // Gravity
        //----------------------------------

        player.vy += GRAVITY;

        //----------------------------------
        // Vertical movement
        //----------------------------------

        {
            Room& room = rooms[currentRoom];

            player.y += player.vy;

            player.onGround = false;

            for (const auto& p : room.platforms)
            {
                if (Intersects(player, p.rect))
                {
                    if (player.vy > 0)
                    {
                        player.y = p.rect.y - player.h;
                        player.vy = 0;
                        player.onGround = true;
                    }
                    else if (player.vy < 0)
                    {
                        player.y = p.rect.y + p.rect.h;
                        player.vy = 0;
                    }
                }
            }
        }

        //----------------------------------
        // Check for room transitions (holes / climb zones live here,
        // since they're only reachable via vertical movement)
        //----------------------------------

        TryExits(rooms[currentRoom], player, currentRoom);

        //----------------------------------
        // Draw
        //----------------------------------

        Room& current = rooms[currentRoom];

        SDL_SetRenderDrawColor(
            renderer,
            current.background.r,
            current.background.g,
            current.background.b,
            255);

        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 90, 60, 20, 255);

        for (const auto& p : current.platforms)
            SDL_RenderFillRect(renderer, &p.rect);

        SDL_Rect playerRect =
        {
            (int)player.x,
            (int)player.y,
            player.w,
            player.h
        };

        SDL_SetRenderDrawColor(renderer, 220, 40, 40, 255);
        SDL_RenderFillRect(renderer, &playerRect);

        SDL_RenderPresent(renderer);

        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}