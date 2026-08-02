I decided while translating this program to C++, to make it very very clear this time how the program works at it's core.

So I have three different versions of the game engine core here.

Step 1, 2, and 3 are completely independent from each other and don't rely on each other.

* Step 1 is a very very fundamental room changing system and text input parser. It is what is fundamentally necessary to make this type of game work at all.

* Step 2 builds on that, and adds a description system for each room, that will work when you change rooms.

* Step 3 adds the look system, and over a hundred little things to look at from across all the rooms.
