#include "Game.h"
using namespace std;
Game::Game() {
    current = world.start;
}

// ======================
// SAVE
// ======================
void Game::save(const string& filename) {
    ofstream out(filename);

    out << "CURRENT " << current->name << "\n";

    out << "INVENTORY " << inventory.size() << "\n";
    for (const auto& item : inventory) {
        out << item << "\n";
    }

    out << "WORLD_ITEMS " << world.nodeMap.size() << "\n";

    for (auto& pair : world.nodeMap) {
        Node* n = pair.second;
        out << n->name << "|";
        if (!n->item.empty())
            out << n->item;
        out << "\n";
    }
}

// ======================
// LOAD
// ======================
void Game::load(const string& filename) {
    ifstream in(filename);
    if (!in) return;

    // Clear inventory first
    inventory.clear();

    // Reset world items
    for (auto& pair : world.nodeMap) {
        pair.second->item = "";
    }

    string line;

    // -------- CURRENT --------
    getline(in, line);
    if (line.rfind("CURRENT", 0) == 0) {
        string name = line.substr(8);
        if (world.nodeMap.count(name)) {
            current = world.nodeMap[name];
        }
    }

    // -------- INVENTORY --------
    getline(in, line);
    int invCount = 0;
    if (sscanf(line.c_str(), "INVENTORY %d", &invCount) == 1) {
        for (int i = 0; i < invCount; i++) {
            string item;
            getline(in, item);
            if (!item.empty())
                inventory.push_front(item);
        }
    }

    // -------- WORLD ITEMS --------
    getline(in, line);
    int nodeCount = 0;
    if (sscanf(line.c_str(), "WORLD_ITEMS %d", &nodeCount) == 1) {
        for (int i = 0; i < nodeCount; i++) {
            string entry;
            getline(in, entry);
            if (entry.empty()) continue;

            size_t bar = entry.find('|');
            if (bar == string::npos) continue;

            string nodeName = entry.substr(0, bar);
            string itemName = entry.substr(bar + 1);

            if (world.nodeMap.count(nodeName)) {
                world.nodeMap[nodeName]->item = itemName;
            }
        }
    }
}
