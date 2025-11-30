// game.h
// Dallas, Kamal, Solomon
#ifndef GAME_H
#define GAME_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include "world.h"
#include "inventory.h"

using namespace std;

void explore(TreeNode* root, ItemNode*& inventory);
bool saveGame(TreeNode* root, TreeNode* current, ItemNode* inventory, const string& filename);
bool loadGame(TreeNode* root, TreeNode*& current, ItemNode*& inventory, const string& filename);

#endif
