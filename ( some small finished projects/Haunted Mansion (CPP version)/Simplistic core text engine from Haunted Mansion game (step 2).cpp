#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>   // for std::tolower


enum Room {
    KITCHEN, FRIDGE, BANQUET_HALL, WASHROOM, BAND_PRACTICE_ROOM, BAND_PRACTICE_CLOSET, ENTRANCE_HALL, UP_ENTRANCE_STAIRWELL, LIBRARY, CREEPY_HALLWAY, MASTER_BEDROOM, PLANT_CONSERVATORY, SHED, PATH_TO_HEDGE_LABYRINTH, HEDGE_LABYRINTH, OBSERVATORY, MANSION_ROOF_LEFT, MANSION_ROOF_RIGHT, LEFT_SPIRE, TOP_OF_LEFT_SPIRE, RIGHT_SPIRE, TOP_OF_RIGHT_SPIRE, PATH_TO_HELIPAD, HELIPAD, NUMBERED_ROOMS, NO_ROOM = -1
};


struct RoomInfo {
    std::string name;
    std::string description;
};

RoomInfo roomInfo[NUMBERED_ROOMS] = {
    /* KITCHEN */
    { "Kitchen",
      "You are in a spooky kitchen, with blood smeared on the floor, and the groaning of zombies. They're clamoring at the window, which has been boarded up with wood and nails. There is a freezer, a counter, and a sink in this kitchen.\n\nTo your north is the banquet hall. To your south is the walk-in fridge." },

    /* FRIDGE */
    { "Fridge",
      "A dank fridge. The food looks a little gross. You notice a key sitting on a shelf." },

    /* BANQUET_HALL */
    { "Banquet Hall",
        "You are in a large empty room, with music playing. Is it... Vivaldi? It's like a large gymnasium. It looks like a dance hall, with long tables through it. There is slamming at the grey double-door doorway. Something wants in, and it wants in bad.\n\nTo the east is the band practice room for the banquet stage. To the north is the washrooms. To the west is the mansion entrance hall." },

    /* WASHROOM */
    { "Washroom",
    "A bathroom with tile floors, golden sink counters, and red walls. There's a bathroom stall, but there's a leg hanging out of it. It doesn't look like a healthy leg, though. In fact, it's green.\n\nYou could knock at the door, or... maybe it's best to just not use the washroom right now? Why was their leg green?\n\nLeave the washroom by walking south." },

    /* BAND_PRACTICE_ROOM */
    { "Band Practice Room",
        "The room is filled with musical instruments, some of which are creepily playing themselves. The black grand piano in the corner echoes with a haunting, repetitive note, D#7. A harp stands next to it. The cello by the closet plays a song by itself, sending chills down your spine. \n\nTo the north is the band closet. To the west is the banquet hall." },

    /* BAND_PRACTICE_CLOSET */
    { "Band Practice Closet",
    "Just an empty storage closet for the most part. Foldable chairs are stacked against one wall, and a coil of piano wire hangs from a hook. Wait... is that a person huddled in the corner? They look absolutely terrified.\n\nTo the south is the band practice room." },

    /* ENTRANCE_HALL */
    { "Entrance Hall",
        "Oh wow, the foyer is massive. Stairwells converge, leading up to another floor, but it's a very large room, with pillars, long red drapes, and a checkered black-and-white floor. And is that... a body? It's lying on the floor next to a torn drape. Large windows reveal the storm outside as rain lashes against the glass.\n\nTo the north is the grand staircase. To the west is the plant conservatory. To the east is the banquet hall." },

    /* UP_ENTRANCE_STAIRWELL */
    { "Upper Entrance Stairwell",
    "You stand at the top of the converging staircase overlooking the mansion's foyer. The railing creaks beneath your hands, and every step echoes through the house.\n\nTo the east is the library. To the north is the observatory. To the west is the creepy hallway. To the south is the entrance hall." },

    /* LIBRARY */
    { "Library",
        "Half of the library has been burnt down, and there seems to be a fort made of books over in the corner. Could there be someone hiding inside?\n\nTo the west is the top of the entrance staircase." },

    /* CREEPY_HALLWAY */
    { "Creepy Hallway",
    "The moment you step into the hallway, you sense that something isn't right. The door slams shut behind you, and the paintings on the walls begin to shake. The hallway twists unnaturally as that screaming returns. Arms reach out from the walls and grab at your ankles. There must be hundreds of them. There isn't much time before those things become violent.\n\nTo the west is the master bedroom. To the east is the top of the staircase." },

    /* MASTER_BEDROOM */
    { "Master Bedroom",
        "The room has a balcony overlooking the conservatory, with thick vines leading from the sill to the ground below. Maybe you could climb down. A plush bed dominates the room, with a wooden dresser and cracked mirror nearby. The sky outside glows an eerie pinkish-orange, like you're trapped inside a nightmare. The cursed knight's armor lies shattered across the floor. Its sword might still be usable.\n\nTo the east is the creepy hallway. You can climb down the vines to the south, reaching the plant conservatory." },

    /* PLANT_CONSERVATORY */
    { "Plant Conservatory",
    "The room is a glass masterpiece, with polygons of glass and steel forming a transparent dome above you. A window is shattered, chilling the garden to an uncomfortable degree. Thorns cover the windows, plants form spires 20ft high, and yet the dome is only illuminated by the moonlight. You can see your breath. \n\n To the west is the pathway to the hedge labyrinth. You could try to climb the vines to the north, that lead up to a balcony. To the east is the entrance hall of the mansion." },

    /* SHED */
    { "Shed",
        "Spiders scurry everywhere as you open the shed. There's garden tools here, but the pitchfork and chainsaw are what catch your eye. Maybe you could take them.\n\nTo go back to the pathway, go north." },

    /* PATH_TO_HEDGE_LABYRINTH */
    { "Path to Hedge Labyrinth",
    "As you leave the conservatory garden, you notice that there's two massive hedges in front of you, with an opening in them. Is that... a maze? Do you dare go in? It starts to the west. To the south, there's a toolshed, presumably for whoever upkeeps the gardens. To go back to the conservatory, go east." },

    /* HEDGE_LABYRINTH */
    { "Hedge Labyrinth",
        "You peer into the opening. This really is a maze. The hedges tower high overhead, blotting out the moonlight. Curious, you venture inside in search of a way through. Hours become days as every path leads you deeper into the labyrinth. Weak from exhaustion, you finally collapse. A pack of zombies finds you before anyone else does.\n\nGame over!" },

    /* OBSERVATORY */
    { "Observatory",
    "An old man stands beside a massive telescope, quietly studying the night sky. The telescope is nearly 20 feet long, and it must be incredibly powerful. Signs point toward the mansion's east and west spires.\n\nTo the west is the left mansion roof. To the east is the right mansion roof. To the south is the top of the entrance staircase. Maybe you should speak to the old man?" },

    /* MANSION_ROOF_LEFT */
    { "Mansion Roof (Left)",
        "You're on the left side of the mansion roof and can finally see the grounds surrounding the estate. The mansion is absolutely swarming with zombies below, their groaning loud enough to carry all the way up here. Beyond the grounds lies a dense forest and a dark lake. From this height, you can also see the hedge labyrinth sprawling into the distance.\n\nTo the west is the left spire. To the north is the pathway to the helipad. To the east is the observatory." },

    /* MANSION_ROOF_RIGHT */
    { "Mansion Roof (Right)",
    "You're on the right side of the mansion roof and can finally see the grounds surrounding the estate. The mansion is absolutely swarming with zombies below, their groaning echoing through the rain. A forest and lake stretch out in the distance. The path to what appears to have been a second helipad has collapsed into a fiery ruin, with the charred remains of a helicopter still smoldering in the storm.\n\nTo the east is the right spire. To the west is the observatory." },

    /* LEFT_SPIRE */
    { "Left Spire",
        "You're at the bottom of a spiral staircase inside the mansion's left spire. The staircase is made of gray stone, winding upward with nothing but open air in the center. One wrong step would send you plummeting to the bottom.\n\nTo the north are the stairs leading upward. To the east is the left side of the mansion roof." },

    /* TOP_OF_LEFT_SPIRE */
    { "Top of Left Spire",
    "After carefully climbing the winding staircase, you emerge into a small guest room tucked inside the spire. A bed, a bookcase, and several narrow castle windows occupy the room. A man stands silently at one of the windows, gazing into the distance.\n\nTo the south are the stairs leading back down the left spire." },

    /* RIGHT_SPIRE */
    { "Right Spire",
        "You're at the bottom of a spiral staircase inside the mansion's right spire. The staircase is made of gray stone, but several steps have crumbled away, leaving dangerous gaps. Broken concrete litters the floor below.\n\nTo the north are the stairs leading upward. To the west is the right side of the mansion roof." },

    /* TOP_OF_RIGHT_SPIRE */
    { "Top of Right Spire",
    "As you near the top of the staircase, a concrete step suddenly snaps beneath your foot. You catch yourself for just a moment.\n\nPhew... that was close—\n\nAHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH!\n\nThe remaining stairs collapse beneath you, sending you crashing all the way back to the bottom of the spire." },

    /* PATH_TO_HELIPAD */
    { "Path to Helipad",
        "You follow a narrow walkway leading across the roof toward the mansion's helipad. Powerful winds buffet you from every direction, while the distant groans of zombies drift upward from below. On the grounds beneath you lies the defeated zombie creature.\n\nTo the north is the helipad. To the south is the left side of the mansion roof." },

    /* HELIPAD */
    { "Helipad",
    "You made it! Five or six exhausted survivors are gathered around a waiting helicopter.\n\n\"Hmm? You need a ride? You haven't been bitten by a zombie, have you? You sure? Alright, climb aboard. We're heading to the military base. Everything's going to be alright, kid. You must be one tough survivor to have made it this far." },
};


struct RoomExits {
    int north;
    int south;
    int west;
    int east;
};

RoomExits worldMap[NUMBERED_ROOMS] = {
    /* KITCHEN */ { BANQUET_HALL, FRIDGE, NO_ROOM, NO_ROOM },
    /* FRIDGE */ { KITCHEN, NO_ROOM, NO_ROOM, NO_ROOM },

    /* BANQUET_HALL */ { WASHROOM, KITCHEN, ENTRANCE_HALL, BAND_PRACTICE_ROOM },
    /* WASHROOM */ { NO_ROOM, BANQUET_HALL, NO_ROOM, NO_ROOM },

    /* BAND_PRACTICE_ROOM */ { BAND_PRACTICE_CLOSET, NO_ROOM, BANQUET_HALL, NO_ROOM },
    /* BAND_PRACTICE_CLOSET */ { NO_ROOM, BAND_PRACTICE_ROOM, NO_ROOM, NO_ROOM },

    /* ENTRANCE_HALL */ { UP_ENTRANCE_STAIRWELL, NO_ROOM, PLANT_CONSERVATORY, BANQUET_HALL },
    /* UP_ENTRANCE_STAIRWELL */ { OBSERVATORY, ENTRANCE_HALL, CREEPY_HALLWAY, LIBRARY },

    /* LIBRARY */ { NO_ROOM, NO_ROOM, UP_ENTRANCE_STAIRWELL, NO_ROOM },
    /* CREEPY_HALLWAY */ { NO_ROOM, NO_ROOM, MASTER_BEDROOM, UP_ENTRANCE_STAIRWELL },

    /* MASTER_BEDROOM */ { NO_ROOM, PLANT_CONSERVATORY, NO_ROOM, CREEPY_HALLWAY },

    /* PLANT_CONSERVATORY */ { MASTER_BEDROOM, NO_ROOM, PATH_TO_HEDGE_LABYRINTH, ENTRANCE_HALL },
    /* SHED */ { PATH_TO_HEDGE_LABYRINTH, NO_ROOM, NO_ROOM, NO_ROOM },

    /* PATH_TO_HEDGE_LABYRINTH */ { NO_ROOM, SHED, HEDGE_LABYRINTH, PLANT_CONSERVATORY },
    /* HEDGE_LABYRINTH */ { NO_ROOM, NO_ROOM, NO_ROOM, PATH_TO_HEDGE_LABYRINTH },

    /* OBSERVATORY */ { NO_ROOM, UP_ENTRANCE_STAIRWELL, MANSION_ROOF_LEFT, MANSION_ROOF_RIGHT },

    /* MANSION_ROOF_LEFT */ { PATH_TO_HELIPAD, NO_ROOM, LEFT_SPIRE, OBSERVATORY },
    /* MANSION_ROOF_RIGHT */ { NO_ROOM, NO_ROOM, OBSERVATORY, RIGHT_SPIRE },

    /* LEFT_SPIRE */ { TOP_OF_LEFT_SPIRE, NO_ROOM, NO_ROOM, MANSION_ROOF_LEFT },
    /* TOP_OF_LEFT_SPIRE */ { NO_ROOM, LEFT_SPIRE, NO_ROOM, NO_ROOM },

    /* RIGHT_SPIRE */ { TOP_OF_RIGHT_SPIRE, NO_ROOM, MANSION_ROOF_RIGHT, NO_ROOM },
    /* TOP_OF_RIGHT_SPIRE */ { NO_ROOM, RIGHT_SPIRE, NO_ROOM, NO_ROOM },

    /* PATH_TO_HELIPAD */ { HELIPAD, MANSION_ROOF_LEFT, NO_ROOM, NO_ROOM },
    /* HELIPAD */ { NO_ROOM, PATH_TO_HELIPAD, NO_ROOM, NO_ROOM },

};

Room currentRoom = KITCHEN;

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
            << roomInfo[currentRoom].name << ".\n\n";
        std::cout << roomInfo[currentRoom].description << "\n\n";
    }
    else {
        std::cout << "You can't go " << direction << " from "
            << roomInfo[currentRoom].name << "!\n\n";
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

    std::cout << "You are in " << roomInfo[currentRoom].name << ".\n\n";
    std::cout << roomInfo[currentRoom].description << "\n\n";
    std::cout << "Type a cardinal NSWE direction to move. Type \"quit\" to quit.\n\n";

    while (true) {
        std::cout << "Input your command: ";
        if (!std::getline(std::cin, line)) break; // Get entire line inputted by user, including if it had spaces in it.

        std::cout << "\n\n";

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
            std::cout << "Say something.\n\n";
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
