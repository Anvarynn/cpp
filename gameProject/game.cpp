// game.cpp
// Dallas, Kamal, Solomon
#include "game.h"
using namespace std;

// ------------------ Explore Loop -------------------
void explore(TreeNode* root, ItemNode*& inventory) 
{
    TreeNode* current = root;
    vector<TreeNode*> path;
    string input;

    cout << "Welcome to the adventure!\n";
    cout << current->name << ": " << current->description << "\n";
    if (!current->item.empty())
    {
         cout << "You see: " << current->item << "\n";
    }

    while (true) 
    {
        cout << "\n> ";
        getline(cin, input);

        // Split command and argument
        string cmd, arg;
        size_t space = input.find(' ');
        if (space != string::npos) 
        {
            cmd = input.substr(0, space);
            arg = input.substr(space + 1);
        } 
        else 
        {
            cmd = input;
        }

        if (cmd == "quit") 
        {
            cout << "Goodbye!\n";
            break;
        } 
        else if (cmd == "look") 
        {
            cout << current->name << ": " << current->description << "\n";
            if (!current->item.empty())
                cout << "You see: " << current->item << "\n";
        } 
        else if (cmd == "left") 
        {
            if (current->left) 
            {
                path.push_back(current);
                current = current->left;
                cout << "Moved left to " << current->name << ".\n";
            } 
            else 
            {
                cout << "No path to the left.\n";
            }
        } 
        else if (cmd == "right") 
        {
            if (current->right) 
            {
                path.push_back(current);
                current = current->right;
                cout << "Moved right to " << current->name << ".\n";
            } 
            else 
            {
                cout << "No path to the right.\n";
            }
        } 
        else if (cmd == "back") 
        {
            if (path.empty()) 
            {
                cout << "No way back.\n";
            } 
            else 
            {
                current = path.back();
                path.pop_back();
                cout << "Moved back to " << current->name << ".\n";
            }
        } 
        else if (cmd == "take") 
        {
            if (current->item.empty()) 
            {
                cout << "No such item here.\n";
            } 
            else if (!arg.empty()) 
            {
                addItem(inventory, current->item);
                current->item.clear();
            } 
            else 
            {
                cout << "Specify an item to take.\n";
            }
        } 
        else if (cmd == "drop") 
        {
            if (current->item.empty()) 
            {
                if (!arg.empty() && removeItem(inventory, arg)) 
                {
                    current->item = arg;
                    cout << "Dropped " << arg << " at " << current->name << ".\n";
                } 
                else 
                {
                    cout << "You don't have that item.\n";
                }
            } 
            else 
            {
                cout << "Place is full.\n";
            }
        } 
        else if (cmd == "inventory") 
        {
            printInventory(inventory);
        } 
        else if (cmd == "traversal") 
        {
            if (arg == "pre") preorder(root);
            else if (arg == "in") inorder(root);
            else if (arg == "post") postorder(root);
            else cout << "Specify traversal type: pre, in, post\n";
        } 
        else if (cmd == "save") 
        {
            string filename = arg.empty() ? "save.txt" : arg;
            if (saveGame(root, current, inventory, filename))
            {
                cout << "Game saved to " << filename << ".\n";
            }
            else
            {
                cout << "Error saving game.\n";
            }
        } 
        else if (cmd == "load") 
        {
            string filename = arg.empty() ? "save.txt" : arg;
            if (loadGame(root, current, inventory, filename)) 
            {
                cout << "Game loaded from " << filename << ".\n";
            }
            else
            {
                cout << "Error loading game.\n";
            }
        } 
        else 
        {
            cout << "Unknown command.\n";
        }
    }
}

// ------------------ Save/Load -------------------
bool saveGame(TreeNode* root, TreeNode* current, ItemNode* inventory, const string& filename) {
    ofstream out(filename);
    if (!out)
    {
      return false;
    }

    out << "CURRENT " << current->name << "\n";

    // Inventory
    int count = 0;
    ItemNode* iter = inventory;
    while (iter) 
    { 
      count++; iter = iter->next; 
    }
    out << "INVENTORY " << count << "\n";
    iter = inventory;
    while (iter) 
    {
        out << iter->itemName << "\n";
        iter = iter->next;
    }

    // World items: preorder traversal
    vector<TreeNode*> nodes;
    function<void(TreeNode*)> collect = [&](TreeNode* n) 
  {
        if (!n) return;
        nodes.push_back(n);
        collect(n->left);
        collect(n->right);
    };
    collect(root);

    out << "WORLD_ITEMS " << nodes.size() << "\n";
    for (TreeNode* n : nodes) 
    {
        out << n->name << "|" << n->item << "\n";
    }

    return true;
}

bool loadGame(TreeNode* root, TreeNode*& current, ItemNode*& inventory, const string& filename) 
{
    ifstream in(filename);
    if (!in)
    {
      return false;
    }

    string line;

    // Build map name->node
    map<string, TreeNode*> nodeMap;
    function<void(TreeNode*)> collect = [&](TreeNode* n) 
    {
        if (!n)
        {
          return;
        }
        nodeMap[n->name] = n;
        collect(n->left);
        collect(n->right);
    };
    collect(root);

    // Clear inventory first
    deleteInventory(inventory);

    // CURRENT
    if (!getline(in, line)) 
    {
      return false;
    }
    if (line.find("CURRENT ") != 0)
    {
      return false;
    }
    string currentName = line.substr(8);
    if (nodeMap.count(currentName)) 
    {
      current = nodeMap[currentName];
    }

    // INVENTORY
    if (!getline(in, line)) 
    {
      return false;
    }
    int invCount = 0;
    sscanf(line.c_str(), "INVENTORY %d", &invCount);
    for (int i = 0; i < invCount; i++) 
    {
        if (!getline(in, line)) 
        {
          break;
        }
        addItem(inventory, line);
    }

    // WORLD_ITEMS
    if (!getline(in, line)) 
    {
      return false;
    }
    int worldCount = 0;
    sscanf(line.c_str(), "WORLD_ITEMS %d", &worldCount);
    for (int i = 0; i < worldCount; i++) 
    {
        if (!getline(in, line)) 
        {
          break;
        }
        size_t bar = line.find('|'); // string::npos if '|' not found
        if (bar == string::npos)
        {
          continue;
        }
        string nodeName = line.substr(0, bar);
        string itemName = line.substr(bar+1);
        if (nodeMap.count(nodeName))
        {
          nodeMap[nodeName]->item = itemName;
        }
    }

    return true;
}
