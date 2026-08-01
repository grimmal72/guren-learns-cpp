#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>


enum class Command {
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
	std::cout << "\n \u0048\u0334\u0308\u0324\u0340\u0352\u0323\u031d\u0336\u0324\u0359\u031d\u030f\u0358\u0336\u0340\u0041\u0334\u0308\u0324\u0340\u0352\u0323\u031d\u0336\u0324\u0359\u031d\u030f\u0358\u0336\u0340\u0055\u0334\u0308\u0324\u0340\u0352\u0323\u031d\u0336\u0324\u0359\u031d\u030f\u0358\u0336\u0340\u004e\u0334\u0308\u0324\u0340\u0352\u0323\u031d\u0336\u0324\u0359\u031d\u030f\u0358\u0336\u0340\u0054\u0334\u0308\u0324\u0340\u0352\u0323\u031d\u0336\u0324\u0359\u031d\u030f\u0358\u0336\u0340\u0045\u0334\u0308\u0324\u0340\u0352\u0323\u031d\u0336\u0324\u0359\u031d\u030f\u0358\u0336\u0340\u0044\u0334\u0308\u0324\u0340\u0352\u0323\u031d\u0336\u0324\u0359\u031d\u030f\u0358\u0336\u0340 \n\n";

	std::cout << "A HORROR GAME\n";
	std::cout << "/////////////////////////////\n";

	std::cout << "This is a text-based interactive fiction game in the style of 1980s DOS games. Type 'help' if you want to see the list of commands.\n";

	std::string line;

    while (true) {
        std::cout << "Input your command: ";
        if (!std::getline(std::cin, line)) break; // EOF / stream closed

        // lowercase it
        std::transform(line.begin(), line.end(), line.begin(),
            [](unsigned char c) { return std::tolower(c); });

        // split into words (similar to parts[0], parts[1], ... in the Java version)
        std::istringstream iss(line);
        std::string firstWord;
        iss >> firstWord;

        Command cmd = parseCommand(firstWord);

        switch (cmd) {
        case Command::North:
        case Command::South:
        case Command::East:
        case Command::West: {
            // your move() logic here — you can still switch again
            // inside on cmd, or pass it to move(Direction)
            std::cout << "Moving...\n";
            break;
        }
        case Command::Look:
            std::cout << "You look around.\n";
            break;
        case Command::Quit:
            std::cout << "Goodbye.\n";
            return 0;
        default:
            std::cout << "I don't understand that.\n";
            break;
        }
    }
}