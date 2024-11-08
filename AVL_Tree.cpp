#include "AVL_Tree.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

int main() {
    AVLTree gatorAVL;
    int numberOfCommands;
    cin >> numberOfCommands;
    cin.ignore();  // Ignore newline character after the number of commands

    vector<string> commands(numberOfCommands);

    // Read each command and store it in the vector
    for (int i = 0; i < numberOfCommands; i++) {
        getline(cin, commands[i]);
    }

    // Process each command
    for (int i = 0; i < numberOfCommands; i++) {
        gatorAVL.processCommand(commands[i]);
    }

    return 0;
}
