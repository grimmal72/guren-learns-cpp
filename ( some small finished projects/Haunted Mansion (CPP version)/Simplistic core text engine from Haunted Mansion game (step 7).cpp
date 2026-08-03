#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cctype>   // for std::tolower
#include <map>

int moves = 0;
int maxMoves = 100;

bool roofGateUnlocked = false;

bool helipadMonsterDefeated = false;
bool bedroomKnightDefeated = false;
bool fridgeZombiesDefeated = false;

enum Room {
    KITCHEN, FRIDGE, BANQUET_HALL, WASHROOM, BAND_PRACTICE_ROOM, BAND_PRACTICE_CLOSET, ENTRANCE_HALL, UP_ENTRANCE_STAIRWELL, LIBRARY, CREEPY_HALLWAY, MASTER_BEDROOM, PLANT_CONSERVATORY, SHED, PATH_TO_HEDGE_LABYRINTH, HEDGE_LABYRINTH, OBSERVATORY, MANSION_ROOF_LEFT, MANSION_ROOF_RIGHT, LEFT_SPIRE, TOP_OF_LEFT_SPIRE, RIGHT_SPIRE, TOP_OF_RIGHT_SPIRE, PATH_TO_HELIPAD, HELIPAD, NUMBERED_ROOMS, NO_ROOM = -1
};


struct RoomInfo {
    std::string name;
    std::string description;
};

// NUMBERED_ROOMS is a holdover, by the way. They're not numbered anymore.
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
    "As you near the top of the staircase, a concrete step suddenly snaps beneath your foot. You catch yourself for just a moment.\n\nPhew... that was close--\n\nAHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHHH!\n\nThe remaining stairs collapse beneath you, sending you crashing all the way back to the bottom of the spire." },

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

std::map<std::string, std::string> roomObjects[NUMBERED_ROOMS];

void defineKitchenObjects() {
    roomObjects[KITCHEN]["counter"] = "It's a wooden kitchen counter. The cutting board is covered in blood. Is that a map in the sink?";
    roomObjects[KITCHEN]["window"] = "The window is covered with thick curtains, blocking out most of the light.";
    roomObjects[KITCHEN]["cupboard"] = "The cupboard is old and creaky, but seems sturdy enough.";
    roomObjects[KITCHEN]["freezer"] = "The freezer is cold to the touch. You can see frost forming around its edges.";
    roomObjects[KITCHEN]["sink"] = "There's a map in the sink.";
    roomObjects[KITCHEN]["map"] = "It's a map. It looks like it's for this building.";
}

void defineFridgeObjects() {
    roomObjects[FRIDGE]["food"] = "There's coleslaw, raw chicken, pork ribs, olives, many condiments, and other foods in this fridge. However, the room isn't very clean.";
    roomObjects[FRIDGE]["zombies"] = "The decapacitated zombies are strewn along the fridge floor.";
    roomObjects[FRIDGE]["key"] = "There is a key on the shelf. Maybe I can use it.";
}

void defineBanquetHallObjects() {
    roomObjects[BANQUET_HALL]["door"] =
        "The slamming is just getting louder and louder. Whatever wants in is scratching, gnawing, and screeching at the door. Wherever you are, it's probably not safe to stay here much longer.";

    roomObjects[BANQUET_HALL]["doors"] =
        "The slamming is just getting louder and louder. Whatever wants in is scratching, gnawing, and screeching at the door. Wherever you are, it's probably not safe to stay here much longer.";

    roomObjects[BANQUET_HALL]["table"] =
        "There are plates and silver cutlery on the tables, over ornate tablecloths. Where am I? Was someone throwing a party here?";

    roomObjects[BANQUET_HALL]["tablecloth"] =
        "There are mandalas and drawings on the tablecloth, kind of like hieroglyphics. It is deep red.";

    roomObjects[BANQUET_HALL]["fork"] =
        "It's a fork.";

    roomObjects[BANQUET_HALL]["knife"] =
        "It's a steak knife.";

    roomObjects[BANQUET_HALL]["wall"] =
        "There is a stage for a band to play on, but there isn't one. The entire room is void of life, but eerily, there's the slamming at the door, and the classical music playing lightly.";
}

void defineBandPracticeRoomObjects() {
    roomObjects[BAND_PRACTICE_ROOM]["piano"] =
        "It's a black grand piano with ivory keys. One note keeps playing. Something is pushing it down. D#7. D#7. D#7. D#7.";

    roomObjects[BAND_PRACTICE_ROOM]["instruments"] =
        "There is a grand piano, a harp, a cello, and many other instruments.";

    roomObjects[BAND_PRACTICE_ROOM]["harp"] =
        "You pluck a string on the harp. Wow, it really does sound angelic.";

    roomObjects[BAND_PRACTICE_ROOM]["cello"] =
        "The cello is playing itself. Creepy.";

    roomObjects[BAND_PRACTICE_ROOM]["closet"] =
        "There is a closet to the north. Maybe there's more instruments in there.";
}

void defineLibraryObjects() {
    roomObjects[LIBRARY]["book"] =
        "You look at a book briefly, but there's no time to stand around.";

    roomObjects[LIBRARY]["books"] =
        "There must have been a fire in here. Half of the room is scorched. Who knows what unfathomable things have happened in this mansion in its past. Perhaps a gas lantern was knocked over while lit? There's scattered books covering almost the entire floor. In the corner is a fort made of books.";

    roomObjects[LIBRARY]["counter"] =
        "The counter is covered in blood, and there's a dead body on it.";

    roomObjects[LIBRARY]["fort"] =
        "There's an opening like a door to the cuboidal fort of books. There's a man inside.";

    roomObjects[LIBRARY]["body"] =
        "Hmm... Maybe it was the librarian?";
}

void defineObservatoryObjects() {
    roomObjects[OBSERVATORY]["telescope"] =
        "You look through the telescope. It's set to look at Jupiter. The gaseous orange hydrogen clouds look beautiful as they shift around each other. What a beautiful night for stargazing. The view is slightly encumbered by the light rain, however.";

    roomObjects[OBSERVATORY]["signs"] =
        "The signs are made of wood. They're on all four doors, saying what room they lead to.";
}

void defineBandPracticeClosetObjects() {
    roomObjects[BAND_PRACTICE_CLOSET]["wire"] =
        "There's a piano wire. It's wrapped up into a circle, but it looks like it could be... four feet?";

    roomObjects[BAND_PRACTICE_CLOSET]["string"] =
        "There's a piano wire. It's wrapped up into a circle, but it looks like it could be... four feet?";

    roomObjects[BAND_PRACTICE_CLOSET]["chair"] =
        "It's a black foldable chair. Pretty average. What's weird is the shaking man huddled up next to the chairs.";

    roomObjects[BAND_PRACTICE_CLOSET]["chairs"] =
        "They're black foldable chairs. Pretty average. What's weird is the shaking man huddled up next to the chairs.";
}

void defineWashroomObjects() {
    roomObjects[WASHROOM]["sink"] =
        "The sink is golden and looks expensive.";

    roomObjects[WASHROOM]["stall"] =
        "There's a leg hanging out of the bathroom stall. It's green and doesn't look healthy.";

    roomObjects[WASHROOM]["mirror"] =
        "The mirror is cracked but you can still see your reflection.";

    roomObjects[WASHROOM]["tiles"] =
        "Typical washroom tiles.";

    roomObjects[WASHROOM]["leg"] =
        "It's green and... necrotic? Is that a zombie in there? Why isn't it moving?";
}

void defineEntranceHallObjects() {
    roomObjects[ENTRANCE_HALL]["body"] =
        "The body is laying down on the floor next to a torn drape. You touch it.";

    roomObjects[ENTRANCE_HALL]["drapes"] =
        "The long red drapes are flowing in the wind coming through the broken window.";

    roomObjects[ENTRANCE_HALL]["floor"] =
        "The checkered black-and-white floor is covered in dust and debris. There's smashed stair railings and a broken pillar crumbled along the ground.";

    roomObjects[ENTRANCE_HALL]["wall"] =
        "Some of the drapes are torn, but what sticks out is the graffiti on the wall that says \"NO ESCAPE\".";

    roomObjects[ENTRANCE_HALL]["doorway"] =
        "The doors are boarded shut with nails and wooden planks. It does not seem safe out there.";
}

void defineUpEntranceStairwellObjects() {
    roomObjects[UP_ENTRANCE_STAIRWELL]["stairs"] =
        "The stairs have lead you up to the next floor.";

    roomObjects[UP_ENTRANCE_STAIRWELL]["landing"] =
        "The landing offers a view of the entrance hall below.";

    roomObjects[UP_ENTRANCE_STAIRWELL]["around"] =
        "The stairs have lead you up to a platform. You have three doors to consider from up here. The signs say that one leads to the library, one leads to the observatory, and one starts towards the master bedroom.";
}

void defineCreepyHallwayObjects() {
    roomObjects[CREEPY_HALLWAY]["paintings"] =
        "The paintings on the wall start shaking as you enter. The faces in the paintings even change, turning malevolent! How is that possible? AAHH!! They're flying at me!";

    roomObjects[CREEPY_HALLWAY]["hands"] =
        "Arms reach out of the walls, grabbing at your ankles. I need to get out of here!";

    roomObjects[CREEPY_HALLWAY]["arms"] =
        "Arms reach out of the walls, grabbing at your ankles. I need to get out of here!";

    roomObjects[CREEPY_HALLWAY]["hallway"] =
        "The hallway is twisting. I don't think I have much time to mess around in this room!";
}

void defineMasterBedroomObjects() {
    roomObjects[MASTER_BEDROOM]["knight"] =
        "It's a good thing I had that piano wire! Looking closer, there's nothing inside of the knight's armor. It must have been cursed.";

    roomObjects[MASTER_BEDROOM]["balcony"] =
        "The balcony looks down at the garden below.";

    roomObjects[MASTER_BEDROOM]["garden"] =
        "It's a nice view.";

    roomObjects[MASTER_BEDROOM]["bed"] =
        "The plush bed looks inviting but now is not the time to rest.";

    roomObjects[MASTER_BEDROOM]["dresser"] =
        "The wooden dresser has a mirror on top. It looks like there's someone behind me in the mirror, but when I turn around, they're not there.";

    roomObjects[MASTER_BEDROOM]["mirror"] =
        "It looks like there's someone behind me in the mirror, but when I turn around, they're not there.";

    roomObjects[MASTER_BEDROOM]["sword"] =
        "It's a large sword. I think I can take it.";
}

void definePlantConservatoryObjects() {
    roomObjects[PLANT_CONSERVATORY]["plants"] =
        "Plants form spires 20 feet high, illuminated by the moonlight. Some of them look like giant venus fly traps that want to eat me.";

    roomObjects[PLANT_CONSERVATORY]["plant"] =
        "Plants form spires 20 feet high, illuminated by the moonlight. Some of them look like giant venus fly traps that want to eat me.";

    roomObjects[PLANT_CONSERVATORY]["vine"] =
        "Although vines cover much of the glass of the dome, the vines that stick out to you are the ones that lead up to a room. It looks like you could climb them.";

    roomObjects[PLANT_CONSERVATORY]["vines"] =
        "Although vines cover much of the glass of the dome, the vines that stick out to you are the ones that lead up to a room. It looks like you could climb them.";

    roomObjects[PLANT_CONSERVATORY]["dome"] =
        "The geodesic glass dome above is impressive. It's made of glass triangular polygons reinforced by steel, but some parts have been smashed through. This mansion has been through a lot.";

    roomObjects[PLANT_CONSERVATORY]["balcony"] =
        "The balcony is part of a room above that looks down upon the garden. I see a dresser and a bed in there.";
}

void definePathToHedgeLabyrinthObjects() {
    roomObjects[PATH_TO_HEDGE_LABYRINTH]["hedges"] =
        "Two massive hedges form the entrance to what seems to be a maze.";

    roomObjects[PATH_TO_HEDGE_LABYRINTH]["maze"] =
        "I have a bad feeling about this...";

    roomObjects[PATH_TO_HEDGE_LABYRINTH]["shed"] =
        "I feel like the toolshed might contain some useful weapons to protect myself.";
}

void defineShedObjects() {
    roomObjects[SHED]["rake"] =
        "A rake leans against the wall. It looks like it has a metal head at least, not plastic. But is this thing really going to help me with anything?";

    roomObjects[SHED]["chainsaw"] =
        "A chainsaw sits on a shelf, looking dangerous. Ooh yeah, baby. Hail to the king.";

    roomObjects[SHED]["spider"] =
        "There's a 14-inch wide spider looking right at you. Uh... Don't make eye contact. Don't make eye contact.";

    roomObjects[SHED]["path"] =
        "The path leads back to where you just were. It's between the garden and the hedge labyrinth. This seems to where the gardener keeps their tools.";
}

void defineMansionRoofLeftObjects() {
    roomObjects[MANSION_ROOF_LEFT]["zombies"] =
        "Zombies swarm the ground below, their groans reaching you even up here.";

    roomObjects[MANSION_ROOF_LEFT]["view"] =
        "This is a fairly secluded area, surrounded by forest. Well, secluded apart from the massive horde of zombies surrounding the building.";

    roomObjects[MANSION_ROOF_LEFT]["spire"] =
        "The outer walls of the mansion look like they're made of dark brick. The spires are tubular and soar to the sky, made of brick themselves.";

    roomObjects[MANSION_ROOF_LEFT]["spires"] =
        "The outer walls of the mansion look like they're made of dark brick. The spires are tubular and soar to the sky, made of brick themselves.";

    roomObjects[MANSION_ROOF_LEFT]["gate"] =
        "There is a locked gate on the way to the helipad.";

    roomObjects[MANSION_ROOF_LEFT]["labyrinth"] =
        "Good thing I didn't go down that hedge maze. It goes on for miles.";

    roomObjects[MANSION_ROOF_LEFT]["maze"] =
        "Good thing I didn't go down that hedge maze. It goes on for miles.";

    roomObjects[MANSION_ROOF_LEFT]["north"] =
        "To your north is the gate that leads to the helipad. You'll need to get through here to escape this mansion.";
}

void defineMansionRoofRightObjects() {
    roomObjects[MANSION_ROOF_RIGHT]["helicopter"] =
        "The remains of a helicopter are smoldering in the rain. You cough a bit on the smoke, which plumes from it.";

    roomObjects[MANSION_ROOF_RIGHT]["zombies"] =
        "Zombies swarm the ground below, their groans reaching you even up here.";

    roomObjects[MANSION_ROOF_RIGHT]["spire"] =
        "The outer walls of the mansion look like they're made of dark brick. The spires are tubular and soar to the sky, made of brick themselves.";

    roomObjects[MANSION_ROOF_RIGHT]["spires"] =
        "The outer walls of the mansion look like they're made of dark brick. The spires are tubular and soar to the sky, made of brick themselves.";

    roomObjects[MANSION_ROOF_RIGHT]["view"] =
        "This is a fairly secluded area, surrounded by forest. Well, secluded apart from the massive horde of zombies surrounding the building.";

    roomObjects[MANSION_ROOF_RIGHT]["helipad"] =
        "The helicopter is destroyed, and the walkway to the helipad has been demolished anyways.";
}

void defineLeftSpireObjects() {
    roomObjects[LEFT_SPIRE]["staircase"] =
        "The spiral staircase looks dangerous and unstable.";

    roomObjects[LEFT_SPIRE]["stairwell"] =
        "The spiral staircase looks dangerous and unstable.";
}

void defineRightSpireObjects() {
    roomObjects[RIGHT_SPIRE]["staircase"] =
        "The spiral staircase is missing steps and littered with broken concrete.";

    roomObjects[RIGHT_SPIRE]["stairwell"] =
        "The spiral staircase is missing steps and littered with broken concrete.";
}

void defineTopOfLeftSpireObjects() {
    roomObjects[TOP_OF_LEFT_SPIRE]["man"] =
        "A man is looking out of the window into the distance.";

    roomObjects[TOP_OF_LEFT_SPIRE]["bed"] =
        "There's a bed against the wall of the room.";

    roomObjects[TOP_OF_LEFT_SPIRE]["bookcase"] =
        "A bookcase filled with old books stands next to the bed.";
}

void definePathToHelipadObjects() {
    roomObjects[PATH_TO_HELIPAD]["path"] =
        "A narrow path leading towards the helipad, with the wind howling around you. The platform is made of the sort of thin metal where you can hear each footstep you take.";

    roomObjects[PATH_TO_HELIPAD]["monster"] =
        "The monster was defeated by your sword. It lies there on the metal. I don't think anything else would have worked, even the chainsaw.";

    roomObjects[PATH_TO_HELIPAD]["zombies"] =
        "Zombies swarm the ground below, their groans reaching you even up here.";

    roomObjects[PATH_TO_HELIPAD]["gate"] =
        "You've unlocked the gate.";

    roomObjects[PATH_TO_HELIPAD]["view"] =
        "You can see the entire mansion and the surrounding forest from here.";
}


std::map<std::string, std::string> peopleObjects[NUMBERED_ROOMS];

void definePeopleObjects() {
    peopleObjects[LIBRARY]["man"] = "\nMan In Book Fort: AAAAAAACCK!! STAY AWAY! Wait, is that a real person? Oh, oh, my dearest apologies. This mansion has been invaded by zombies and ransacked by raiders so many times, now, I'm just a little... fatigued. Oh, who am I, you ask? I was the gardener for this mansion. I'm holding out for just one more hour. I felt safe in this mansion when things started falling apart, but it just keeps getting crazier and crazier. There's a rescue team coming to take us to a military base. What, you don't know? The world's been overrun by zombies, kid. It's been two years. Anyways, I don't care who you are. If you wanna get to safety, get on that helicopter. It'll be leaving from the west helipad, on the roof. Hopefully I'll see you there shortly.\n";
    peopleObjects[BAND_PRACTICE_CLOSET]["man"] = "\nMan In Closet: Wh-what are you looking at? It's not safe out there. Just leave me alone. Take that piano wire from the shelf above me. It might come in handy.\n";
    peopleObjects[OBSERVATORY]["man"] = "\nOld Man: Ah... A guest. I was just looking at the stars. They look very beautiful tonight. A comet is going by tonight, as well. Hm... I think there's a rescue helicopter coming tonight, but I think I'll stay here and watch the stars. Good luck. Would you like to use the telescope? Go ahead if you'd like.\n";
    peopleObjects[TOP_OF_LEFT_SPIRE]["man"] = "\nMansion Owner: My, my. What a beautiful view. The destruction is immaculate. Hm... You didn't hear that, did you? Hm... Who am I? Hahahaha! Who are you? This is my mansion. How did you get in here? Irregardless. You look harrowed. You want out of here, don't you? I think there's some monsters roaming about. You know how to defeat them, right? If you fight them with something... meager, you will inevitably lose in combat, yes? You look like you can only carry one weapon at a time. Choose your weapon wisely, to prevail in combat. Good luck. There's a helicopter leaving in an hour. Get out of my mansion. Leave me to admire the view.\n";
}


// These next two bool functions are used for the inventory.
bool takeKeyInputted = false;
bool hasKey() {
	return takeKeyInputted;
}

bool takeMapInputted = false;
bool hasMap() {
    return takeMapInputted;
}

void useMap() {
    std::cout << "                                           Helipad\n";
    std::cout << "                                              |\n";
    std::cout << "                 Top of Left Spire      Path to Helipad                                        Top of Right Spire\n";
    std::cout << "                             |                |                                                         |\n";
    std::cout << "                       Left Spire --- Mansion Roof Left --- Observatory --- Mansion Roof Right --- Right Spire\n";
    std::cout << "                                                                 |\n";
    std::cout << "                      Master Bedroom --- Creepy Hallway --- Top of Stairs --- Library           Band Room Closet\n";
    std::cout << "                                                                 |                                    |\n";
    std::cout << "Hedge Labyrinth --- Path to Hedge Labyrinth --- Garden --- Entrance Hall ----- Banquet Hall ----- Band Room\n";
    std::cout << "                                 |                                                  |\n";
    std::cout << "                              Toolshed                                           Kitchen\n";
    std::cout << "                                                                                    |\n";
    std::cout << "                                                                                  Fridge\n";
}

enum Weapon {
	NO_WEAPON,
	WIRE,
	SWORD,
	RAKE,
	CHAINSAW,
    KNIFE
};

Weapon currentWeapon = NO_WEAPON;

bool hasWeapon() {
	return currentWeapon != NO_WEAPON; // If currentWeapon is not equal to NO_WEAPON, it means the player has a weapon, and the function returns true.
}

void dropWeapon() {
    currentWeapon = NO_WEAPON;
    std::cout << "You dropped your weapon.\n";
}

void pickUpWeapon(const std::string& weaponName) {
    if (hasWeapon()) {
        std::cout << "You already have a weapon in your hands. Drop it first.\n";
        return;
    }

    if (weaponName == "chainsaw") {
        currentWeapon = CHAINSAW;
        std::cout << "You picked up the chainsaw.\n";
    }
    else if (weaponName == "knife") {
        currentWeapon = KNIFE;
        std::cout << "You picked up the knife.\n";
    }
    else if (weaponName == "rake") {
        currentWeapon = RAKE;
        std::cout << "You picked up the rake.\n";
    }
    else if (weaponName == "string" || weaponName == "wire" ||
        weaponName == "piano string" || weaponName == "piano wire") {
        currentWeapon = WIRE;
        std::cout << "You picked up the piano wire.\n";
    }
    else if (weaponName == "sword") {
        currentWeapon = SWORD;
        std::cout << "You picked up the sword.\n";
    }
    else {
        std::cout << "Invalid weapon.\n";
    }
}

// This is here only for the weapon drop sequence, for a single line of the game. It is here so that when you drop the weapon, the specific weapon is said, but where you don't see the weird enum, such as SWORD, or WIRE, or CHAINSAW. It should be lowercase instead. And so now it is. But also, just saying "wire" instead of "piano wire" is a bit vague, which I guess is why we're doing this instead of just using the tolower() method. 
std::string weaponToString(Weapon w) {
    switch (w) { // "w" could have been "weapon". This is a parameter that is passed into the function.
    case CHAINSAW: return "chainsaw";
    case KNIFE:    return "knife";
    case RAKE:     return "rake";
    case WIRE:     return "piano wire";
    case SWORD:    return "sword";
    default:       return "nothing";
    }
}



Room currentRoom = KITCHEN;


// Note: The only reason this is bool is for the moves++ system in another part of the program. It provides a true or false for whether a room was walked into, or if tryMove() went to NO_ROOM. If it went to NO_ROOM, then the player didn't move, and so the moves++ system doesn't increment.
bool tryMove(const std::string& direction) {
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

		return true; // The player successfully moved to a new room.
    }
    else {
        std::cout << "You can't go " << direction << " from "
            << roomInfo[currentRoom].name << "!\n\n";

        return false; // The player didn't move to a new room.
    }
}

enum Command {
    North, South, East, West,
    Look, Map, Inventory, Quit,
    Unknown
};

Command parseCommand(const std::string& word) {
    if (word == "north" || word == "n") return Command::North;
    if (word == "south" || word == "s") return Command::South;
    if (word == "east" || word == "e") return Command::East;
    if (word == "west" || word == "w") return Command::West;
    if (word == "look" || word == "l") return Command::Look;
    if (word == "map" || word == "m") return Command::Map;
    if (word == "inventory" || word == "i") return Command::Inventory;
    if (word == "quit" || word == "q") return Command::Quit;
    return Command::Unknown;
}

int main() {
    defineKitchenObjects();
    defineFridgeObjects();
    defineBanquetHallObjects();
    defineBandPracticeRoomObjects();
	defineLibraryObjects();
    defineObservatoryObjects();
	defineBandPracticeClosetObjects();
    defineWashroomObjects();
	defineEntranceHallObjects();
    defineUpEntranceStairwellObjects();
	defineCreepyHallwayObjects();
    defineMasterBedroomObjects();
    definePlantConservatoryObjects();
	definePathToHedgeLabyrinthObjects();
    defineShedObjects();
	defineMansionRoofLeftObjects();
    defineMansionRoofRightObjects();
	defineLeftSpireObjects();
    defineRightSpireObjects();
	defineTopOfLeftSpireObjects();
    definePathToHelipadObjects();

    definePeopleObjects();


    std::string line;

    std::cout << "You are in " << roomInfo[currentRoom].name << ".\n\n";
    std::cout << roomInfo[currentRoom].description << "\n\n";
    std::cout << "Type a cardinal NSWE direction to move. Type \"quit\" to quit.\n\n";

    while (true) {
        if (moves >= maxMoves) {
            // This is kind of like a time limit. If you spend too much time roaming around
            // in the game, the moves counter will keep incrementing until the maximum
            // amount. If it reaches the max amount, the zombies break in and it's game
            // over.
            std::cout <<
                "\nCrk. Crk. *KSSSSSSH!!!* Did a window break? The screaming is getting louder. Some creatures are coming from the darkness. Whoa, zombies? Ah, AH, THEY'RE EVERYWHERE! AAAAACGGGGGGHHHHHHHH!!! One bites you! You fall to the floor and bleed out. Game over!\n\n";
            return 0;
        }

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
                if (tryMove("north")) {
                    moves++;
                }
                break;
            case Command::South:
                if (tryMove("south")) {
                    moves++;
                }
                break;
            case Command::East:
                if (tryMove("east")) {
                    moves++;
                }
                break;
            case Command::West:
                if (tryMove("west")) {
                    moves++;
                }
                break;
            case Command::Look:
                std::cout << "You look around.\n";
                std::cout << roomInfo[currentRoom].description << "\n\n";
                break;
            case Command::Map:
                if (hasMap()) {
                    useMap();
				}
				else {
					std::cout << "You don't have a map.\n";
				}
                break;
            case Command::Inventory:
                std::cout << "You check your inventory.\n";
				std::cout << "Weapon: " << (hasWeapon() ? "You have a " + weaponToString(currentWeapon) + " in your hands.\n" : "You have no weapon in your hands.\n") << "\n";
				std::cout << "Map: " << (hasMap() ? "You have a map.\n" : "You don't have a map.\n") << "\n";
				std::cout << "Key: " << (hasKey() ? "You have a key.\n" : "You don't have a key.\n") << "\n";
                break;
            case Command::Quit:
                std::cout << "Goodbye.\n";
                return 0;
            default:
                std::cout << "I don't understand that.\n";
                break;
            }
        }
        else { // parts.size() >= 2
            std::string verb = parts[0];
            std::string subject = parts.back();

            if (verb == "look") {
                std::map<std::string, std::string>& objects = roomObjects[currentRoom];
             
                std::map<std::string, std::string>::iterator found = objects.find(subject);
             
                bool wasFound = (found != objects.end());
                // objects.end() is the place after the map data structure ends, in kind of an out-of-bounds area. So if the iterator actually found something first, then it won't try to attach onto end(), the out-of-bounds area of the map.

                if (wasFound) {
                    std::cout << found->second << "\n";
                }
                else {
                   std::cout << "You don't see a " << subject << " here.\n";
                }
            }
            else if (verb == "talk") {
                std::map<std::string, std::string>& people = peopleObjects[currentRoom];

                std::map<std::string, std::string>::iterator found = people.find(subject);

                bool wasFound = (found != people.end());

                if (wasFound) {
                    std::cout << found->second << "\n";
                }
                else {
                    std::cout << "You don't see a " << subject << " here.\n";
                }
            }
            else if (verb == "take") {
                if (subject == "chainsaw") {
                    if (currentRoom == SHED) {
                        pickUpWeapon("chainsaw");
                    }
                    else {
                        std::cout << "There is no chainsaw in this room!\n";
                    }
                }
                else if (subject == "knife") {
                    if (currentRoom == BANQUET_HALL) {
                        pickUpWeapon("knife");
                    }
                    else {
                        std::cout << "There is no knife in this room!\n";
                    }
                }
                else if (subject == "rake") {
                    if (currentRoom == SHED) {
                        pickUpWeapon("rake");
                    }
                    else {
                        std::cout << "There is no rake in this room!\n";
                    }
                }
                else if (subject == "sword") {
                    if (currentRoom == MASTER_BEDROOM) {
                        pickUpWeapon("sword");
                    }
                    else {
                        std::cout << "There is no sword in this room!\n";
                    }
                }
                else if (subject == "string" || subject == "wire") {
                    if (currentRoom == BAND_PRACTICE_CLOSET) {
                        pickUpWeapon("wire");
                    }
                    else {
                        std::cout << "There is no piano wire in this room!\n";
                    }
                }
                // Non-weapon items
				else if (subject == "map") { // Not to be confused, of course, with the data structure.
					if (currentRoom == KITCHEN) {
						std::cout << "You take the map from the sink.\n";
                        takeMapInputted = true;
					}
					else {
						std::cout << "There is no map in this room!\n";
					}
				}
				else if (subject == "key") {
					if (currentRoom == FRIDGE) {
						std::cout << "You take the key from the shelf.\n";
                        takeKeyInputted = true;
					}
					else {
						std::cout << "There is no key in this room!\n";
					}
				}
                else {
                    std::cout << "You can't take that.\n";
                }
            }
            else if (verb == "drop") {
                if (subject == "weapon") {
                    // Generic "drop weapon" always works if you're holding anything
                    if (hasWeapon()) {
                        std::string weaponName = weaponToString(currentWeapon);
                        dropWeapon();
                        std::cout << "You dropped the " << weaponName << ".\n";
                    }
                    else {
                        std::cout << "There is no weapon to drop!\n\n";
                    }
                }
                else if ((subject == "wire" || subject == "string") && currentWeapon == WIRE) {
                    dropWeapon();
                    std::cout << "You dropped the piano wire.\n";
                }
                else if (subject == weaponToString(currentWeapon) && hasWeapon()) {
                    // Covers chainsaw / knife / rake / sword
                    std::string weaponName = weaponToString(currentWeapon);
                    dropWeapon();
                    std::cout << "You dropped the " << weaponName << ".\n";
                }
                else if (subject == "chainsaw" || subject == "knife" || subject == "rake" ||
                    subject == "sword" || subject == "wire" || subject == "string") {
                    // The user typed a real weapon name, but that's not what they're holding
                    std::cout << "You don't have a " << subject << " to drop.\n";
                }
                else {
                    std::cout << "You can't drop that.\n";
                }
            }
            else {
                std::cout << "I don't understand that.\n";
            }
        }
    }

    return 0;
}
