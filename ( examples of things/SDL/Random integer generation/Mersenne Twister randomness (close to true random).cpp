#include <iostream>
#include <random>

int main() {
    // Set up a random number generator
    std::random_device rd;                          // seed source
    std::mt19937 gen(rd());                          // Mersenne Twister engine
    std::uniform_int_distribution<int> dist(1, 100);  // range: 1 to 100 (inclusive)

    int randomNumber = dist(gen);
    std::cout << "Random number: " << randomNumber << std::endl;

    return 0;
}

// Note: I hate the above, so I'll have to thoroughly learn it somewhere, or scrap this and switch back to C rand().
// // For the previous project of a simple Gachapon system, I need a randomness generator, by the way.