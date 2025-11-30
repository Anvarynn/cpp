#include "World.h"
using namespace std;
// ----------------------
// Constructor / Destructor
// ----------------------
World::World() {
    createWorld();
}

World::~World() {
    destroyWorld();
}

// ----------------------
// Build the example world
// ----------------------
void World::createWorld() {
    Node* a = new Node("Forest Entrance");
    Node* b = new Node("Dark Cave");
    Node* c = new Node("Sunny Meadow");

    // Manually link them
    a->north = b;
    b->south = a;
    b->east = c;
    c->west = b;

    start = a;

    // Build map for quick lookup:
    nodeMap[a->name] = a;
    nodeMap[b->name] = b;
    nodeMap[c->name] = c;
}

// ----------------------
// Delete world nodes
// ----------------------
void World::destroyWorld() {
    for (auto& pair : nodeMap) {
        delete pair.second;
    }
    nodeMap.clear();
}
