#ifndef WORLD_H
#define WORLD_H

#include <string>
#include <map>
#include <iostream>
using namespace std;

// ---------------------------
// Struct representing a node
// ---------------------------
struct Node {
    string name;
    string item;  // Empty string = no item
    Node* north = nullptr;
    Node* south = nullptr;
    Node* east = nullptr;
    Node* west = nullptr;

    Node(const string& n) : name(n), item("") {}
};

// ---------------------------
// World class
// ---------------------------
class World {
public:
    Node* start = nullptr;
    map<string, Node*> nodeMap;  // Maps name → pointer

    World();
    ~World();

    void createWorld();
    void destroyWorld();
};

#endif
