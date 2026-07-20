#include <iostream>
#include <string>
#include <format>

#include <cstdlib>
#include <ctime>
using namespace std;

int shotCounter = 0;
int swingValue = 0;
int holeDistance = 1120;

bool running = true;

string scoreNames[9] = { "Hole-in-one!", "Albatross.", "Eagle.", "Birdie.", "Par.", "Bogey.", "Double Bogey...", "Triple Bogey...", "Worse than Triple Bogey... Basically, really bad!" };

int coursePar = 5; // this is passed to 'par' in golfScore()
int shots = 0;


string swingAdjective = "";
string distanceAdjective = "";
string tournamentStanding = "";
string fancyScoreWord = "";

string golfScore(int shots, int par) {
	if (shots == 1) {
		return scoreNames[0];
	}
	else if (shots == par - 3) {
		return scoreNames[1];
	}
	else if (shots == par - 2) {
		return scoreNames[2];
	}
	else if (shots == par - 1) {
		return scoreNames[3];
	}
	else if (shots == par) {
		return scoreNames[4];
	}
	else if (shots == par + 1) {
		return scoreNames[5];
	}
	else if (shots == par + 2) {
		return scoreNames[6];
	}
	else if (shots == par + 3) {
		return scoreNames[7];
	}
	else {
		return scoreNames[8];
	}
}

void endingSequence() {

	fancyScoreWord = golfScore(shots, coursePar);

	string scoreTextFormattedString = format("Ball in hole! Your score is: {}\n\n", fancyScoreWord);

	cout << scoreTextFormattedString;

	if (fancyScoreWord == scoreNames[0] || fancyScoreWord == scoreNames[1] || fancyScoreWord == scoreNames[2]) {
		cout << "You came in 1st! Congratulations!! You win the game! You are Golf Chad! Your name goes down in history! The guy in bronze looks at you contemptuously, for he is not Golf Chad.\n\n";
	}
	else if (fancyScoreWord == scoreNames[3] || fancyScoreWord == scoreNames[4]) {
		cout << "You came in 2nd! So close! You'll be remembered, but not as much as the other guy.\n\n";
	}
	else if (fancyScoreWord == scoreNames[5] || fancyScoreWord == scoreNames[6]) {
		cout << "You came in 3rd. You feel as if you should have won. The event guy sneers at you as he hands you your trophy. But hey, at least you're on the podium, right?\n\n";
	}
	else {
		cout << "You lose! You had a chance at the gold medal, at the prestigious Golf Island Cup, but you blew it. You so blew it. The crowd laughs, and you're handed a $5 gift card. Golf Chad, on the other hand, is paid ten million dollars.\n\n";
	}

	running = false;
}

	

void swing() {
	swingValue = std::rand() % 500;

	holeDistance = holeDistance - swingValue;

	shotCounter++;

	shots = shotCounter;

	if (holeDistance <= 0) {
		cout << "You shot it right in from there! Wow!\n\n";
		endingSequence();
	}

	if (50 >= swingValue) {
		swingAdjective = "petty";
	}
	else if (150 >= swingValue && swingValue > 50) {
		swingAdjective = "stunning";
	}
	else if (225 >= swingValue && swingValue > 150) {
		swingAdjective = "fantastic";
	}
	else {
		swingAdjective = "spectacular";
	}

	if (40 >= holeDistance) {
		distanceAdjective = "meager";
	}
	else if (250 >= holeDistance && holeDistance > 40) {
		distanceAdjective = "modest";
	}
	else if (600 >= holeDistance && holeDistance > 250) {
		distanceAdjective = "substantial";
	}
	else if (900 >= holeDistance && holeDistance > 600) {
		distanceAdjective = "considerable";
	}
	else {
		distanceAdjective = "cool";
	}

	if (holeDistance > 0) {
		std::string holeDistanceFormattedString = std::format("You hit a {} {} feet! You have a {} {} feet remaining.\n\n", swingAdjective, swingValue, distanceAdjective, holeDistance);

		cout << holeDistanceFormattedString;
	}
}

int main() {
	cout << "It's a beautiful sunny day on Golf Island. The cobalt waters break upon the coastline. There is no wind. You've done a 20-hole course on the green. There's only one match to go. Par is 6.\n\nIf you get a Birdie (par minus 1), you'll win the cup! You stand at the tee and get ready to strike.  The hole is 1120 feet away. You wind up your club.\n\n";

	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	while (running) {
		swing();
	}

	return 0;
}