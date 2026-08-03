I decided while translating this program to C++, to make it very very clear this time how the program works at it's core.

So I have four different versions of the game engine core here.

Step 1, 2, 3, 4 and 5 are completely independent from each other and don't rely on each other.

* Step 1 is a very very fundamental room changing system and text input parser. It is what is fundamentally necessary to make this type of game work at all.

* Step 2 builds on that, and adds a description system for each room, that will work when you change rooms.

* Step 3 adds the "look" system, and over a hundred little things to look at across all the rooms.

* Step 4 adds the "talk" system, which uses the exact same verb, subject, and map/object code template as the "look" system.

* Step 5 adds the full weapon system, including picking up, dropping, and having weapon pickups only occur in the room that the weapon is found in. However, it doesn't include the weapon-related scripts for what happens when the boss fights occur, as the boss fights haven't been added yet.
