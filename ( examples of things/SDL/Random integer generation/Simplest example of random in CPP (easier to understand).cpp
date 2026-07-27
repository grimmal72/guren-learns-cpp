#include <cstdio>
#include <cstdlib>
#include <ctime>

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // seed once

    int minVal = 1;
    int maxVal = 100;
    int randomNumber = std::rand() % (maxVal - minVal + 1) + minVal;

    std::printf("Random number: %d\n", randomNumber);

    return 0;
}