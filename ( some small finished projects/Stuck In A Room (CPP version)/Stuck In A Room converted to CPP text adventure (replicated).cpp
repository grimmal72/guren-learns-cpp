#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

enum Passage {
    WAKE_UP,
    LOOK_WINDOW,
    USE_DOOR,
    DOOR_TRY_OPEN,
    DOOR_KNOCK,
    DOOR_EXAMINE,
    VENDING_MACHINE,
	PEBBLES_COLA,
	ORANGE_ADE,
	LOCAL_JOES_ROOT_BEER,
	SWEDISH_CHERRY_CHUG,
	PICK_UP_PAPER,
	MOVE_THE_VENDING_MACHINE,
    CRAWL_THROUGH_THE_HOLE,
    LOOK_MIRROR,
    GET_WATER,
    EXAMINE_AIRSHAFT,
    EXAMINE_CLOSER,
    EXAMINE_KEY,
    NUM_PASSAGES
};

// As an example of how Choice works, in { "Look at the window", LOOK_WINDOW }, the label is the text the player sees, and the target is the enum path it uses.
struct Choice {
    std::string label; // "Look at the window"
    Passage target; // where it leads
};

// As an example of how PassageInfo works, in { "You're in a small room. There's a door and a window.", { { "Look at the door", LOOK_DOOR }, { "Look at the window", LOOK_WINDOW } } }, there are two parts to the array. The first half is the text description you would see if you were in that "passage"/"room". Then, the second half of what you can see in the array, is another array (really a vector array) of passage choices, which are, on any "screen", the options of where you could go.
struct PassageInfo {
    std::string text; // the text dump for each "room"
    std::vector<Choice> choices;
};

PassageInfo passages[NUM_PASSAGES];

void definePassages() {
    std::cout << "Stuck In A Room - a text-adventure style game\n\n";

    passages[WAKE_UP] = {
        "You wake up with a shiver. As your eyes open and close and things unblur, you see that "
        "you're in a white room with painfully bright white lights. The bed you're laying on is "
        "hard and there are springs digging into your sides. You have no blanket.\n\n"
        "You sit upright, and plop your calloused dirty feet on the cold tile floor. You also "
        "notice you have bruises on your arms. There is a hum from the tube lights above you.\n\n"
        "You're (stuck?) in a small room with a dented metal door, a toilet, a sink with a mirror, "
        "a TV, some paintings, a closed window, and some sort of vending machine.",
        {
            { "Look at the window",          LOOK_WINDOW },
            { "Use the door out",            USE_DOOR },
            { "Check out the vending machine", VENDING_MACHINE },
            { "Look in the mirror",          LOOK_MIRROR },
            { "Get some water",              GET_WATER },
        }
    };

    passages[LOOK_WINDOW] = {
        "The window is glass, but also has bars in front of it.\n\n"
        "So you can't see outside or be heard, but you can't just break the glass to get out either.\n\n"
        "You wonder what kind of sickos would lock a person inside a room like this.",
        {}   // dead end, back is the only way out
    };

    passages[USE_DOOR] = {
        "You step to the door, but abruptly, a sliding viewport opens and clangs with a metal sound.\n\n"
        "\"Wot are you doin', prisoner? You know what you did. Now stay in your little pen. Hahahaha!\"\n\n"
        "He spits at you and slides the viewport shut.",
        {
            { "Try to open the door anyway.", DOOR_TRY_OPEN },
            { "Knock",                        DOOR_KNOCK },
            { "Examine",                      DOOR_EXAMINE },
        }
    };

    passages[DOOR_TRY_OPEN] = {
        "There's a door handle on your side, but the door won't budge. It's more of a lever than a knob, so you jump up and down to put the force of your bodyweight down upon it, but the lock is sturdy.\n\n"
        "The guard opens the metal viewport.\n\n"
        "\"Think we're shtoopid, eh? What kind of guard would I be if I left the door open? Here, have some crumbs, I've just eaten.\"\n\n"
        "He blows crumbs in your face and slams the viewport shut.",
        {}
    };

    passages[DOOR_KNOCK] = {
        "You knock.\n\n"
        "\"Sod off!\"\n\n"
        "He doesn't even open the viewport. He just winds up and snaps the door with his guardstick, creating a loud stinging clank on your end.",
        {}
    };

    passages[DOOR_EXAMINE] = {
        "The door is made of a thick-looking slate of metal. It has dents all over it, dirty marks, and near the floor to the left of it, the white wall has a spattering of blood across it.\n\n"
        "Have you been the only one trapped in here?",
        {}
    };






    passages[VENDING_MACHINE] = {
        "Although you're a bit distressed about not knowing what's going on, you have a faint smile when you walk up to the vending machine.\n\n"
        "It's a pop machine.\n\n"
        "You can get: ",
        {
            { "Pebbles Cola", PEBBLES_COLA },
            { "Orange-Ade", ORANGE_ADE },
            { "Local Joe's Root Beer", LOCAL_JOES_ROOT_BEER },
            { "Swedish Cherry Chug", SWEDISH_CHERRY_CHUG },
        }
    };

    passages[PEBBLES_COLA] = {
        "It tastes like cola, but not very flavorful. You wonder why someone would even bother to make cola if it's just going to be tasteless.",
        {}
    };

    passages[ORANGE_ADE] = {
        "You're not sure if it tastes like orange juice or orange soda. Orange soda would have a bubbly carbonation and sort of an artificial flavor, or juice would taste like juice.\n\n"
        "But this just tastes weird.\n\n"
        "As you drink your orange-ade, you look at the machine. It has a giant picture of a man in a top hat winking. It says, 'Drink Your Sugar'.\n\n"
        "You notice a piece of paper sticking out a bit underneath the machine.",
        { { "Pick it up?", PICK_UP_PAPER } }
    };

    passages[LOCAL_JOES_ROOT_BEER] = {
        "You crack it open and notice it has a picture of a man in a dog costume as the logo mascot. What the hell? What is Local Joe into?\n\n"
        "It tastes like garbage.\n\n"
        "'How does someone get the recipe for root beer wrong?', you think. 'Isn't it just syrup?'",
        {}
    };

    passages[SWEDISH_CHERRY_CHUG] = {
        "This one is the sweetest of them all. It's brutally sweet. Viciously sweet. You think you might be having a heart attack!!\n\n"
        "Oh, wait, that's just the warm feeling it gives you in your stomach. Must be a bit of heartburn, too...\n\n"
        "You think this is probably your favorite soda here, despite that it nearly killed you.",
        {}
    };

    passages[PICK_UP_PAPER] = {
        "You read the piece of paper:\n\n"
        "----------------------------------------------\n"
        "'Day 612:\n\n"
        "If you're reading this, you have amnesia. You won't know why you're here. It seems that the guards have beaten you silly, and your long-term memory is shot. Your, that is, my, memory seems to reset on the daily. I read these notes and am reminded of whatever new revelations I may have written down for myself.\n\n"
        "I still don't know how to get out of here, but I've carved a hole out behind this machine, small enough for a skinny person to crawl through.\n\n"
        "There's also a key hidden below the sink in the airshaft vent. You'll need that later.\n\n"
        "I think I've managed to not get caught so far when I go behind the vending machine. The guards don't seem to pay you any mind, really, even when giving you food. They just throw it through the hole and slam the door shut.\n\n"
        "I've left more notes along the way. Good luck!\n\n"
        "Sincerely,\n"
        "Yourself (I don't know our name)\n\n"
        "PS. The day counter may also be inaccurate, as there will be days that you don't find this piece of paper, but it's a rough estimate. For all I know it could be day ten thousand.'\n"
        "----------------------------------------------\n\n"
        "You're taken aback. A letter from yourself? You write letters to yourself every day? You've been in here almost two years, maybe more? You don't know what to think.\n\n"
        "You decide to grab that key before coming back to move the vending machine.",
        {
            { "Examine the airshaft", EXAMINE_AIRSHAFT },
            { "Move the vending machine", MOVE_THE_VENDING_MACHINE },
        }
    };

    passages[MOVE_THE_VENDING_MACHINE] = {
        "You ready yourself to move the vending machine by squatting down and grabbing the legs, and it very slowly budges. You make a loud tension sound like:\n\n"
        "\"Eeeeeeeeeeiiiiiiiaaaaaaaaaaahhhhhhh!!\" which piques the guard's interest."
        "\"Ayy, mate, that's nasty. Make sure you flush, or...\" He knocks with the guardstick against the door. \"I'll beat you into having a little more respect. Hehe...\""
        "You manage to pull the vending machine back enough to slip through behind it and see the hole. It's really rough, and there is rubble everywhere. It may have taken you a very long time to carve through this wall.",
        {
            { "Crawl through the hole", CRAWL_THROUGH_THE_HOLE },
        }
    };

    passages[CRAWL_THROUGH_THE_HOLE] = {
        "(Note: In the original version, this area was never written.)",
        {}
    };

    passages[LOOK_MIRROR] = {
        "The mirror is dirty and cracked, but you can see yourself. Your face is bruised, and you have a black eye. Your arms are also bruised, and your clothing is shabby. For some reason your feet are really calloused too.\n\n"
        "How long have you been here? What's going on here?",
        {}
    };

    passages[GET_WATER] = {
        "You don't think anyone's going to give you any water, so you just twist your body and "
        "neck to drink some water mouth-to-faucet from the sink.\n\n"
        "You're glad that at least there even is a sink.\n\n"
        "As you step back from the sink, you notice that there's an airshaft under it.",
        {
            { "Examine the airshaft", EXAMINE_AIRSHAFT },
        }
    };




    passages[EXAMINE_AIRSHAFT] = {
        "There is a small airshaft near the ground. It's missing one screw, so you rotate it around it's hinge. You could kick it in, but it seems unrefined.\n\n"
        "You can see the airshaft unobscured by the ventplate now. The passage is only a foot wide and half a foot tall. You're quite skinny from being in this weird place, presumably without proper nourishment, but you still can't fit in there.",

        {
            {"Examine closer", EXAMINE_CLOSER },
        }
    };

    passages[EXAMINE_CLOSER] = {
        "All that you can see is kind of an L-shaped passage that goes two feet deep and then turns to the left.\n\n"
        "You have a feeling there could be something around that corner, so you lay flat on the floor and reach around the passage.\n\n"
        "You feel something around the bend, on the floor of it. It makes a scratchy noise when it slides. Is it a coin? It's... a key?",

        {
            {"Examine the key", EXAMINE_KEY },
        }
    };

    passages[EXAMINE_KEY] = {
        "It's a small doorkey, like to a house.\n\n"
        "'What a crazy place this is', you think to yourself.\n\n"
        "You feel a compulsion to go to the vending machine.",

        {
            {"Check out the vending machine", VENDING_MACHINE },
        }
    };
}

int main() {
    definePassages();

    Passage currentPassage = WAKE_UP;
    std::vector<Passage> history;   // acts as the "back button"

    while (true) {
        PassageInfo& p = passages[currentPassage];

        std::cout << "\n" << p.text << "\n\n";

        // This is not an input system for the list of choices, it is just the list of choices.
        for (size_t i = 0; i < p.choices.size(); i++) {
            std::cout << (i + 1) << ". " << p.choices[i].label << "\n";
        }
        // This isn't an input system for 0, it is still part of the list of choices.
        if (!history.empty()) {
            std::cout << "0. Go back\n";
        }

        std::cout << "\n> ";

        // This is the input system.
        std::string input;
        if (!std::getline(std::cin, input)) break; // std::cin on its own does not allow you to make a line string with spaces, so you must use std::getline.

        // This section replaces the previous system, which was "int choice = std::atoi(input.c_str());", which would allow something like a letter or random symbols to return 0. This doesn't have that problem.
        int choice;
        try {
            choice = std::stoi(input);
        }
        catch (const std::exception&) { // This exception is a little weird, but basically we are making the exception object immutable, and we are referring to it by reference.
            choice = -1;
        }

        if (choice == 0 && !history.empty()) {
            currentPassage = history.back();
            history.pop_back();
        }
        else if (choice >= 1 && choice <= (int)p.choices.size()) {
            history.push_back(currentPassage);
            currentPassage = p.choices[choice - 1].target;
        }
        else {
            std::cout << "\nInvalid choice.\n";
        }
    }

    return 0;
}