#include <iostream>
#include <string>
#include <sstream>  // stringstream, necessary for interpreting what the user typed
#include <vector>
#include <cctype>   // for std::tolower, the lowercasing system for what the user typed

// This program is a completely functional prototype of what a room system combined with a cin text parsing engine would look like in C++. The room changing works.
// And the typing system works. You can type nothing, and it will tell you to type, or you can type one word, or you can type 2+ words. You can also type gibberish
// and it will just tell you it doesn't understand you, and then it will let you try again. This is about as simplistic as this can get.
// And it works very well.

enum Room {
    ROOM1, ROOM2, ROOM3, ROOM4,
    ROOM5, ROOM6, ROOM7, ROOM8,
    NUMBERED_ROOMS,
    NO_ROOM = -1
};

std::string roomNames[NUMBERED_ROOMS] = {
    "Room 1", "Room 2", "Room 3", "Room 4",
    "Room 5", "Room 6", "Room 7", "Room 8"
};

struct RoomExits {
    int north;
    int south;
    int east;
    int west;
};

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

void tryMove(const std::string& direction) {
    int next = NO_ROOM;
    RoomExits& exits = worldMap[currentRoom];

    if (direction == "north")
    {
        next = exits.north;
    }

    else if (direction == "south")
    {
        next = exits.south;
    }

    else if (direction == "east")
    {
        next = exits.east;
    }

    else if (direction == "west")
    {
        next = exits.west;
    }

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

enum Command {
    North, South, East, West,
    Look, Inventory, Quit,
    Unknown
};

Command parseCommand(const std::string& word) {
    if (word == "north" || word == "n") return Command::North;
    if (word == "south" || word == "s") return Command::South;
    if (word == "east" || word == "e") return Command::East;
    if (word == "west" || word == "w") return Command::West;
    if (word == "look" || word == "l") return Command::Look;
    if (word == "inventory" || word == "i") return Command::Inventory;
    if (word == "quit" || word == "q") return Command::Quit;
    return Command::Unknown;
}

int main() {
    std::string line;

    std::cout << "You are in " << roomNames[currentRoom] << ".\n";
    std::cout << "Use arrow keys to move. Esc to quit.\n\n";

    while (true) {
        std::cout << "Input your command: ";
        if (!std::getline(std::cin, line)) break; // Get entire line inputted by user, including if it had spaces in it.

        // lowercase it
        for (size_t i = 0; i < line.size(); i++) {
            line[i] = std::tolower(line[i]);
        }

        // split into words (equivalent to parts[0], parts[1], ... in the Java version)
        std::istringstream iss(line);
        std::vector<std::string> parts;
        std::string word;
        while (iss >> word) {
            parts.push_back(word);
        }

        if (parts.empty()) {
            std::cout << "Say something.\n";
        }
        else if (parts.size() == 1) {
            Command cmd = parseCommand(parts[0]);

            switch (cmd) {
            case Command::North:
                tryMove("north");
                break;
            case Command::South:
                tryMove("south");
                break;
            case Command::East:
                tryMove("east");
                break;
            case Command::West: {
                tryMove("west");
                break;
            }
            case Command::Look:
                std::cout << "You look around.\n";
                break;
            case Command::Inventory:
                std::cout << "You check your inventory.\n";
                break;
            case Command::Quit:
                std::cout << "Goodbye.\n";
                return 0;
            default:
                std::cout << "I don't understand that.\n";
                break;
            }
        }
		
		// Note: the below else statements and all of it's contents does not need to be uncommented for this program to run. Just leave the "else" empty inside if you want.
		// The below code also doesn't work right now. But the program above all completely works. The else logic below is useful though since it has the verb and subject
		// logic for parsing a sentence such as "look at the man" and focusing on the first word and the last word (which will usually be the verb and the subject).
		// If the sentence has some bizarre structure, the program would run anyways, it would just say it doesn't understand, and then it would let you go again.
		
        else { //// parts.size() >= 2
            //std::string verb = parts[0];
            //std::string subject = parts.back();   // last element, same idea as parts[parts.length - 1]

            //if (verb == "look") {
            //    currentRoom.lookAt(subject);
            //}
            //else if (verb == "drop") {
            //    if (subject == "chainsaw" || subject == "knife" || subject == "string" ||
            //        subject == "rake" || subject == "sword" || subject == "weapon") {
            //        playerWeapon.dropWeapon();
            //        std::cout << "You dropped the weapon.\n";
            //    }
            //    else {
            //        std::cout << "The map and key aren't droppable. Weapons are droppable. "
            //            "Other than that, there's nothing to drop!\n";
            //    }
            //}
            //else if (verb == "talk") {
            //    if ((currentRoom == bandPracticeCloset || currentRoom == library ||
            //        currentRoom == observatory || currentRoom == topOfLeftSpire) &&
            //        subject == "man") {
            //        if (currentRoom == bandPracticeCloset) {
            //            currentRoom.lookAt("man");
            //        }
            //        else if (currentRoom == observatory) {
            //            currentRoom.lookAt("man");
            //        }
            //        // ...rest of your talk logic
            //    }
            //}
            //else {
            //    std::cout << "I don't understand that.\n";
            //}
        }
    }
}
