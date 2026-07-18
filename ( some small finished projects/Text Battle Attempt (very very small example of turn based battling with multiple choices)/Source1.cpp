#include <iostream>
using namespace std;

bool playerGameOverStatus;

class Player {
public:
	int health = 100;
	int attackPower = 12;
};

class Enemy {
public:
	string name;
	int health;
	int attackPower;

	Enemy(string name, int health, int attackPower)
		: name(name), health(health), attackPower(attackPower) { }
};

class Slime : public Enemy {
public:
	Slime() : Enemy("Slime", 30, 3) {}
};

class Rat : public Enemy {
public:
	Rat() : Enemy("Rat", 20, 5) {}
};

class Goblin : public Enemy {
public:
	Goblin() : Enemy("Goblin", 50, 8) {}
};


void doBattle(Player& player, Enemy& enemy) {
	cout << "You encounter a " << enemy.name << ". It has " << enemy.health << " health.\n\n";
	cout << "It attacks you!\n";
	player.health -= enemy.attackPower;
	cout << "Your health: " << player.health << "\n";
	cout << "What will you do?\n";

	cout << "In battle, type \"Attack\" or \"a\" to attack an enemy.\n";

	cout << "Or, type \"Wait\" or \"w\" to skip a turn.\n";

	while (true) {

		if (enemy.health <= 0) {
			cout << "You beat the " << enemy.name << "!\n";
			
			break;
		}

		if (player.health <= 0) {
			playerGameOverStatus = true;
			break;
		}

		while (true) { // Your attack turn loop.
			string cinInput;
			cin >> cinInput;

			if ((cinInput == "Attack") || (cinInput == "a")) {
				enemy.health -= player.attackPower;
				cout << "You deal " << player.attackPower << " damage. The "
					<< enemy.name << "'s health is now " << enemy.health << ".\n";
				break;
			}
			else if ((cinInput == "Wait") || (cinInput == "w")) {
				cout << "You skip a turn.\n";
				break;
			}
			else {
				cout << "That is not a valid command. Try again.\n";
			}
		}

		while (true) { // The enemy's attack turn loop.
			cout << "The " << enemy.name << " attacks you! It deals " << enemy.attackPower << " damage!\n";

			player.health -= enemy.attackPower;

			cout << "Your health is now " << player.health << "!\n";

			break;
		}
	}
}


int main() {
	// Instantiate objects
	Player player;
	Slime slime;
	Rat rat;
	Goblin goblin;

	cout << "You are exploring a dark and mysterious cave you found from a hole in the ground next to a fallen tree in the forest.\n\n";

	cout << "You are a brave warrior, albeit a bit spooked. Your health is " << player.health << ".\n\n";



	
		
		doBattle(player, slime);
		if (playerGameOverStatus) goto gameOver;

		doBattle(player, rat);
		if (playerGameOverStatus) goto gameOver;

		doBattle(player, goblin);


gameOver:
	cout << "You have died. The kingdom has lost it's hero, and will perish. Game over.\n";

	return 0;
};