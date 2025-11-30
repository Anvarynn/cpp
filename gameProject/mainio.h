// Dallas, Kamal, Solomon
// Main IO test
#ifndef MAINIO_H
#define MAINIO_H
#include <iostream>
#include <string>
#include "world.h"
#include "inventory.h"
#include "gameio.h"
using namespace std;

void main_test_io() 
{
    cout << "===== AdventureQuest Integration Test =====\n";

    // 1. Build world
    TreeNode* root = createWorld();
    TreeNode* current = root;

    // 2. Inventory setup
    ItemNode* inventory = nullptr;
    addItem(inventory, "torch");
    addItem(inventory, "sword");

    // 3. Print starting info
    cout << "\nStarting at: " << current->name << "\n";
    cout << "Inventory:\n";
    printInventory(inventory);

    // 4. Move around manually
    if (current->left) 
    {
      current = current->left;
    }
    cout << "\nMoved left to: " << current->name << "\n";
    if (current->right) 
    {
      current = current->right;
    }
    cout << "Moved right to: " << current->name << "\n";

    // 5. Take and drop simulation
    if (!current->item.empty()) 
    {
        addItem(inventory, current->item);
        current->item = "";
        cout << "Picked up item at current location.\n";
    }

    // 6. Save game
    bool saved = saveGame(root, current, inventory, "test_save.txt");
    cout << "\nSave game " << (saved ? "succeeded." : "failed.") << "\n";

    // 7. Reset current & inventory
    current = root;
    deleteInventory(inventory);
    inventory = nullptr;

    // 8. Load game
    bool loaded = loadGame(root, current, inventory, "test_save.txt");
    cout << "Load game " << (loaded ? "succeeded." : "failed.") << "\n";

    // 9. Verify loaded state
    cout << "\nCurrent location after load: " << (current ? current->name : "NULL") << "\n";
    cout << "Inventory after load:\n";
    printInventory(inventory);

    // 10. Traversals test
    cout << "\nPreorder traversal:\n";
    preorder(root);
    cout << "\nInorder traversal:\n";
    inorder(root);
    cout << "\nPostorder traversal:\n";
    postorder(root);

    // 11. Clean up
    deleteInventory(inventory);
    deleteTree(root);

    cout << "\n===== End of Test =====\n";
}

#endif
