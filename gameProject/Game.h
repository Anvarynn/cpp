#ifndef GAME_H
#define GAME_H

#include "World.h"
#include <string>
#include <list>
#include <fstream>
#include <sstream>
using namespace std;

// ---------------------------
// Game class
// ---------------------------
class Game {
public:
    World world;
    Node* current;
    list<string> inventory;

    Game();

    void save(const string& filename);
    void load(const string& filename);
};

#endif
