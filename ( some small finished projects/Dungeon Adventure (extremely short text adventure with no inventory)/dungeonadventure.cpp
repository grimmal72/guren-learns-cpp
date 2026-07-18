#include <iostream>
#include <format> // Required for std::format
#include <limits> // Required for std::numeric_limits

int main()
{
    // INTRO NARRATIVE TEXT
    std::cout << "You are a brave warrior, exploring the dusty ruins below a forest entryway you found above. You have been wandering in these ruins for what feels like forever now. Thankfully, your torch has managed to hold flame so far. Suddenly, you hear a noise. A rat screeches and runs, then footsteps come closer. You grab the hilt of your sword. Is it a spider? An orc? The footsteps continue, and a face starts to come out of the darkness. It is a gaunt, old, human face, with a bump on the nose. They continue to step closer and you now notice that it is an old wise-looking man with a beard. Dirty, though. He is wearing a long filthy cap. But, why didn't he have a torch or some illumination for himself? Can he see down here? Is he used to the darkness? You wonder to yourself what is going on. You also now notice that you had subconsciously raised your sword into a wound-up position as if ready to attack. You lower your sword to your side gently.\n\n";

    std::cout << "The man speaks. \"Oh-ho, flighty, aren't we? What have I done to you to deserve being sliced into bits? ...I forgive you, lad. Well, what brings you down to these ruins? I don't see much in the way of humans down here very often. Mostly just those... tasty rats and spiders and snakes. Mm...\" He slowly licks his lips, then pulls his tongue back into his mouth.\n\n";

    int choice = 0;
    int shillings = 500;

    bool firstDialoguePickedOptionTwo = false;
    bool firstDialoguePickedOptionThree = false;

    while (true)
    {

        // FIRST OPTIONS LIST, A LIST OF 3 NUMBERS WITH THINGS YOU COULD SAY
        std::cout << "You decide to speak. Type a number key into the console.\n1) You... you eat the creatures down here?\n2) I am searching for the 6th orb of Urr. I have to rescue the princess. Have you heard what is happening above land?\n3) I'll give you 5 shillings if you turn around, walk away, and I never have to see your emaciated face again.\n\n";

        std::cin >>
            choice;

        if (choice == 1)
        {
            std::cout << "The man speaks. \"Hmm? What else am I supposed to eat down here? I've been stuck down here for 15 years.\" He grabs a crawling spider from the wall, and starts chomping on it. You wince.\n\n";

            std::cout << "He wastes no part of the spider, eating it in it's entirety. You try to look away for a moment, but end up watching him eat it completely.\n\n";

            continue;
        }
        else if (choice == 2)
        {
            std::cout << "The man speaks. \"I have heard whispers in the caves from the goblins about it. Tell me, did the dark lord destroy the kingdom?\"\n\n";

            std::cout << "You nod.\n\n";

            std::cout << "\"I see. And you are collecting the 7 orbs of Urr to concentrate the power of the goddesses into the fabled temple of light, I suppose?\"\n\n";

            std::cout << "You nod.\n\n";

            std::cout << "\"I see. You are a noble man indeed. ...But I am not.\" He grabs a snake from the wall, and bites it's head off. Blood leaks all over his beard, his hand, and the ground. \"Give me 100 shillings. Then I will guide you to the temple of spirits.\"\n\n";

            firstDialoguePickedOptionTwo = true;
            break;
        }
        else if (choice == 3)
        {
            std::cout << "\"5 shillings?\" He snatches it out of your hands. \"Good luck, young one.\" He drifts back into the darkness, and fades away. It is now just you and your torch again. However, you soon become hopelessly lost. Your torch runs out of flame, you struggle to find water while using your hands to follow the walls, and you drop out of exhaustion in the darkness two days later. A giant spider eats your corpse after you die of thirst.\n\n";

            firstDialoguePickedOptionThree = true;
            break;
        }
        else
        {
            std::cout << "Invalid choice. Type a number key that you see listed.\n";
            std::cin.clear();                                                   // clears the fail state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // dumps the bad input
            // those two above lines are AI generated.
            continue;
        }
    }

    // RESULTING IF ELSE CHAIN BASED ON THE ABOVE CHOICE

    // I think I'll just not add a path for option 1. So the first if is for option 2.
    if (firstDialoguePickedOptionTwo == true)
    {

        while (true)
        {
            std::cout << "Give him 100 shillings?\n\n1) Yes\n2) No\n\n";

            choice = 0;
            std::cin >> choice;

            if (choice == 1)
            {
                shillings = shillings - 100;
                std::cout << std::format("You give him the 100 shillings. You now have {} shillings.\n\n", shillings);

                std::cout << "The man speaks. \"Good man. I can buy some clean water for myself with this for some time. I'll lead you to the temple.\"\n\n";

                std::cout << "The man leads you to the spirit temple, and through grit and perserverance, you later destroy the demon ghost king of this temple and retrieve the 6th orb of Urr.\n\n";

                std::cout
                    << "Ultimately, you retrieve the 7th orb of Urr, bring them all to the temple of light, and develop the power of the goddesses necessary to defeat the dark lord and restore light to the kingdom. You save the kingdom, marry the princess, and the kingdom lives happily ever after.\n\n";

                std::cout
                    << "YOU WIN! YOU SAVED THE PRINCESS AND THE KINGDOM!\n\n";

                std::cout << "Press any key to exit.";

                std::cin.ignore();
                std::cin.get();
                break;
            }
            else if (choice == 2)
            {
                std::cout << "The man speaks. \"I see. The miserly type, are we? Good with saving your shillings? That's fine. I'm so fargone with being down here that I am not above being unhelpful to a hero. Good luck with your adventure.\" He throws the half-eaten snake against the wall, and walks off into the darkness.\n\n";

                std::cout << "A few hours later, your torch runs out of flame. You are submerged in darkness. You attempt to find your way around the caves by guiding your hands along the walls, but you accidentally step into a cavernous hole, and fall to your death. Rats feed on your mangled corpse.\n\n";

                std::cout << "GAME OVER! YOU HAVE DIED! THE DARK LORD OVERTAKES THE KINGDOM, AND THE PRINCESS WILL NEVER BE SAVED.\n\n";

                std::cout << "Press any key to exit.";

                std::cin.ignore();
                std::cin.get();
                break;
            }
            else
            {
                std::cout << "Invalid choice. Type a number key that you see listed.\n";

                continue;
            }
        }
    }
    else if (firstDialoguePickedOptionThree == true)
    {
        std::cout << "GAME OVER! YOU HAVE DIED! THE DARK LORD OVERTAKES THE KINGDOM, AND THE PRINCESS WILL NEVER BE SAVED.\n\n";

        std::cout << "Press any key to exit.";

        std::cin.ignore();
        std::cin.get();
    }

    return 0;
}