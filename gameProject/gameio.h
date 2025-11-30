// Dallas, Kamal, Solomon
// Game test
#ifndef GAMEIO_H
#define GAMEIO_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "world.h"
#include "inventory.h"
using namespace std;

// Save game state to a text file
bool saveGame(TreeNode* root, TreeNode* current, ItemNode* inventory, const string& filename) 
{
    if (!root || !current) 
    {
      return false;
    }

    ofstream out(filename);
    if (!out.is_open()) 
    {
      return false;
    }

    // Save current location
    out << "CURRENT " << current->name << "\n";

    // Save inventory
    int invCount = 0;
    ItemNode* temp = inventory;
    while (temp) 
    { 
      invCount++; temp = temp->next; 
    }
    out << "INVENTORY " << invCount << "\n";
    temp = inventory;
    while (temp) 
    {
        out << temp->itemName << "\n";
        temp = temp->next;
    }

    // Save world items: traverse tree and collect nodes
    vector<TreeNode*> nodes;
    function<void(TreeNode*)> collect = [&](TreeNode* node) 
    {
        if (!node) 
        {
          return;
        }
        nodes.push_back(node);
        collect(node->left);
        collect(node->right);
    };
    collect(root);

    out << "WORLD_ITEMS " << nodes.size() << "\n";
    for (auto node : nodes) 
    {
        out << node->name << "|" << node->item << "\n";
    }

    out.close();
    return true;
}

// Load game state from a text file
bool loadGame(TreeNode*& root, TreeNode*& current, ItemNode*& inventory, const string& filename) 
{
    if (!root) 
    {
      return false;
    }
    ifstream in(filename);
    if (!in.is_open()) 
    {
      return false;
    }

    string line;
    unordered_map<string, TreeNode*> nodeMap;

    // Build node map for quick lookup
    function<void(TreeNode*)> mapNodes = [&](TreeNode* node) 
    {
        if (!node) 
        {
          return;
        }
        nodeMap[node->name] = node;
        mapNodes(node->left);
        mapNodes(node->right);
    };
    mapNodes(root);

    // Clear existing inventory
    deleteInventory(inventory);
    inventory = nullptr;

    while (getline(in, line)) 
    {
        if (line.rfind("CURRENT ", 0) == 0) 
        {
            string name = line.substr(8);
            if (nodeMap.count(name)) 
            {
              current = nodeMap[name];
            }
        }
        else if (line.rfind("INVENTORY ", 0) == 0) 
        {
            int count = stoi(line.substr(10));
            for (int i = 0; i < count; i++) 
            {
                if (getline(in, line)) 
                {
                  addItem(inventory, line);
                }
            }
        }
        else if (line.rfind("WORLD_ITEMS ", 0) == 0) 
        {
            int count = stoi(line.substr(12));
            for (int i = 0; i < count; i++) 
            {
                if (!getline(in, line)) 
                {
                  break;
                }
                size_t bar = line.find('|');
                if (bar == string::npos) 
                {
                  continue;
                }
                string nodeName = line.substr(0, bar);
                string itemName = line.substr(bar + 1);
                if (nodeMap.count(nodeName)) 
                {
                  nodeMap[nodeName]->item = itemName;
                }
            }
        }
    }

    in.close();
    return true;
}

#endif
