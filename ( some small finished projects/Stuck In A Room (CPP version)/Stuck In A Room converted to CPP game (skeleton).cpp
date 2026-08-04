#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>   // for std::atoi

enum Passage {
    START,
    LOOK_DOOR,
    OPEN_DOOR,
    LOOK_WINDOW,
    KNOCK,
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
    passages[START] = {
        "You're in a small room. There's a door and a window.",
        {
            { "Look at the door",   LOOK_DOOR }, // Basically, this is a "Choice".
            { "Look at the window", LOOK_WINDOW }, // And this is a "Choice".
        }
    };

    passages[LOOK_DOOR] = {
        "The door is metal, with a small slot near the top.",
        {
            { "Open the door", OPEN_DOOR },
            { "Knock",         KNOCK },
        }
    };

    passages[OPEN_DOOR] = {
        "It's locked. Of course it is.",
        {}   // dead end -- "go back" is the only option
    };

    passages[KNOCK] = {
        "Nobody answers. Just an echo.",
        {}   // dead end
    };

    passages[LOOK_WINDOW] = {
        "The glass is frosted. You can't see anything through it.",
        {}   // dead end
    };
}

int main() {
    definePassages();

    Passage currentPassage = START;
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

        int choice = std::atoi(input.c_str());

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