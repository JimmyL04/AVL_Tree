
#ifndef AVL_TREE_AVL_H
#define AVL_TREE_AVL_H
#endif //AVL_TREE_AVL_H
#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Node{
    string name;
    string ufid;
    Node* left;
    Node* right;
    int height;

    Node(string n, string u) :
    name(n),
    ufid(u),
    left(nullptr),
    right(nullptr),
    height(1) {}
};


class AVLTree{

private:
    Node* root;

    int countNodes(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        // Recursively count nodes in
        return 1 + countNodes(node->left) + countNodes(node->right);
    }

    int getHeight(Node* node){ //Referenced from GeeksfroGeeks.org (https://www.geeksforgeeks.org/insertion-in-an-avl-tree/)
        if (node == nullptr){
            return 0;
        }
        return node->height;
    }

    int getBalance(Node* node){ //Referenced from GeeksfroGeeks.org (https://www.geeksforgeeks.org/insertion-in-an-avl-tree/)
        if (node == nullptr){
            return 0;
        }
        return getHeight(node->left ) - getHeight(node->right);
    }

    Node* findMinimum(Node* node){
        Node* current =node;
        while (current->left != nullptr){
            current = current->left;
        }
        return current;
    }

    //Left & Right Rotations
    //From Slides
    Node* rotateLeft (Node* node){
        Node* newParent = node->right;
        Node* grandChild = newParent->left;

        //Rotating
        newParent->left = node;
        node->right = grandChild;

        //Updating Height
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        newParent->height = 1 + max(getHeight(newParent->left), getHeight(newParent->right));
        return newParent;
    }

    Node* rotateRight(Node* node){
        Node* newParent = node->left;
        Node* grandChild = newParent->right;

        //Rotating
        newParent->right = node;
        node->left = grandChild;

        //Updating Height
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
        newParent->height = 1 + max(getHeight(newParent->left), getHeight(newParent->right));
        return newParent;
    }



    //Helper because I need help
    //Referenced from GeeksfroGeeks.org (https://www.geeksforgeeks.org/insertion-in-an-avl-tree/)
    Node* InsertHelper(Node* node, string name, string ufid) {
        if (node == nullptr) {
            return new Node(name, ufid);
        }

        if (ufid < node->ufid) {
            node->left = InsertHelper(node->left, name, ufid);
        }
        else if (ufid > node->ufid) {
            node->right = InsertHelper(node->right, name, ufid);
        }
        else {
            return node; //For Duplicate UFIDs
        }
        //Update height of the ancestor node
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        //Checking for balance and different cases
        int balance = getBalance(node);

        //Left Left
        if (balance > 1 && ufid < node->left->ufid) {
            return rotateRight(node);
        }

        //Right Right
        if (balance < -1 && ufid > node->right->ufid) {
            return rotateLeft(node);
        }

        //Left Right
        if (balance > 1 && ufid > node->left->ufid) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        //Right Left
        if (balance < -1 && ufid < node->right->ufid) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node; //return node pointer
    }



    Node* deleteHelper(Node* node, string ufid) {
        if (node == nullptr) {
            // Base case
            return nullptr;
        }

        // Navigate the tree to find the node to delete
        if (ufid < node->ufid) {
            node->left = deleteHelper(node->left, ufid);
        }

        else if (ufid > node->ufid) {
            node->right = deleteHelper(node->right, ufid);
        }

        else {
            // Node with only one child or no child
            if (node->left == nullptr || node->right == nullptr) {
                Node* temp = node->left ? node->left : node->right;

                // No child case
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                }
                else {
                    // One child Policy
                    *node = *temp;
                }
                delete temp;
            }

            else {
                // Node with two children: Get the inorder successor (smallest in the right subtree)
                Node* temp = findMinimum(node->right);

                // Copy the inorder successor's data to this node and then delete the successor
                node->name = temp->name;
                node->ufid = temp->ufid;

                // Call deleteHelper for the right subtree to delete the inorder successor
                node->right = deleteHelper(node->right, temp->ufid);
            }
        }

        if (node == nullptr) {
            // If the tree had only one node then return
            return node;
        }

        // Update height of the current node
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        return node;
    }



    //Traversals
    void inOrder(Node* node, vector<Node*>& nodes) {
        if (node != nullptr) {
            inOrder(node->left, nodes);
            nodes.push_back(node);
            inOrder(node->right, nodes);
        }
    }


    void preOrder(Node* node, vector<Node*>& nodes) {
        if (node != nullptr) {
            nodes.push_back(node);
            preOrder(node->left, nodes);
            preOrder(node->right, nodes);
        }
    }


    void postOrder(Node* node, vector<Node*>& nodes) {
        if (node != nullptr) {
            postOrder(node->left, nodes);
            postOrder(node->right, nodes);
            nodes.push_back(node);
        }
    }


    //Checking UFIDs
    bool validUFID(const string& ufid){
        if (ufid.length()!= 8){ //Making sure that the UFID is at least 8 digits in length
            return false;
        }
        for (char i : ufid){
            if(!isdigit(i)){
                return false;
            }
        }
        return true;
    }

    bool validName(const string& name){ // Iterate through each character in the provided name
        for (char c : name) {
        // Check if the character is a letter (A-Z or a-z) or a space
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == ' ')) {
            // If a character is found that is neither a letter (uppercase or lowercase) nor a space,
            // the name is considered invalid, so return false.
            return false;
        }
    }
    // If all characters are valid, return true
    return true;
    }

    bool nodeExists(Node* node, const string& ufid) {
        if (node == nullptr) {
            return false; // Base case: reached the end of the path without finding the ufid.
        }
        if (ufid == node->ufid) {
            return true; // The ufid is found in the current node.
        }
        else if (ufid < node->ufid) {
            return nodeExists(node->left, ufid); // Search the left subtree.
        }
        else {
            return nodeExists(node->right, ufid); // Search the right subtree.
        }
    }


    Node* removeNode(Node* node) {

        if (node == nullptr){
            return nullptr;
        }

        // Node with only one child or no child
        if (node->left == nullptr || node->right == nullptr) {
            Node* temp = (node->left != nullptr) ? node->left : node->right;

            // No child case
            if (temp == nullptr) {
                delete node;  // Simply delete the node
                node = nullptr;  // Set the node to nullptr after deletion
            }

            else {
                // One child case: Replace node with its single child
                Node* tempNode = node;  // Temporarily store node to be deleted
                node = temp;  // Replace node with its non-null child
                delete tempNode;  // Delete the old node

            }
        }

        else {
            // Node with two children: Get the inorder successor (smallest in the right subtree)
            Node* temp = findMinimum(node->right);

            // Copy the inorder successor's data to this node
            node->name = temp->name;
            node->ufid = temp->ufid;

            // Delete the inorder successor from the right subtree
            node->right = deleteHelper(node->right, temp->ufid);
        }

        // If the node was deleted and is now null, just return null
        if (node == nullptr){
            return nullptr;
        }

        // Update height
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        return node;
    }


    // Removal helper for AVL Tree
   Node* removeNthInorderHelper(Node*& node, int& N) {
        if (node == nullptr) return nullptr;  // Base case

        // Traverse the left subtree
        if (N > 0) {
            node->left = removeNthInorderHelper(node->left, N);
            // If N is decremented in the left subtree, we've found and removed the target
            if (N == 0) return node;
        }

        // If this node is the target, remove it
        if (N == 0) {
            Node* temp = removeNode(node);
            N--;  // Decrement N as this node is now being removed
            return temp;  // Return the node that was removed
        }

        // Only decrement N if we're still looking for our target
        if (N > 0) {
            N--;
        }

        // If N still > 0 after decrement, it means target is in right subtree
        if (N >= 0) {
            node->right = removeNthInorderHelper(node->right, N);
        }

        // Update height and balance
        if (node) {
            node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        }
        return node;
    }



    //UFID Search Name
    // Helper method to traverse the tree and find all UFIDs with the given name
    void searchByNameHelper(Node* node, const string& name, bool& found) {
        if (node == nullptr) {
            return;
        }
        // Inorder traversal: left, process, right
        searchByNameHelper(node->left, name, found);  // Search in the left subtree
        // Check if the current node's name matches the search name
        if (node->name == name) {
            cout << node->ufid << endl;  // Print the UFID associated with this name
            found = true;
        }
        searchByNameHelper(node->right, name, found);  // Search in the right subtree
    }

    //Destructor to handle memeory leaks
    void treeDestructor(Node* node){
        if (node != nullptr){
            treeDestructor(node->left);
            treeDestructor(node->right);
            delete node;
            node = nullptr;
        }
    }

public:
    void searchByID(const string& ufid) {
        Node* current = root;
        while (current != nullptr) {
            if (ufid == current->ufid) {
                cout << current->name << endl;  // Found the UFID, print the associated name.
                return;
            }

            else if (ufid < current->ufid) {
                current = current->left;  // Continue searching in the left subtree.
            }

            else {
                current = current->right;  // Continue searching in the right subtree.
            }
        }
        cout << "unsuccessful" << endl;  // The UFID was not found in the tree.
    }


    void searchByName(const string& name) {
        bool found = false;  // Flag to indicate whether we've found at least one matching node
        searchByNameHelper(root, name, found);  // Start the search from the root
        if (!found) {
            cout << "unsuccessful" << endl;  // If no nodes with the given name were found
        }
    }


    void processCommand(const string& command) {
        string action;
        string details;
        int index = command.find_first_of(' '); //Referenced from (https://www.educative.io/answers/how-to-use-the-findfirstof-function-in-cpp)
        action = command.substr(0, index);
        details = command.substr(index + 1);

        //Checking for User Command

        if (action == "insert") {
            int nameEnd = details.find_first_of('\"', 1);
            string name = details.substr(1, nameEnd - 1); // Extracting the name from the command.
            string ufid = details.substr(nameEnd + 2); // Extracting the UFID from the command.

            if (validUFID(ufid) && validName(name)) {
                if (!nodeExists(root, ufid)) { // Check if a node with this UFID already exists.
                    root = InsertHelper(root, name, ufid); // Insert the new node.
                    cout << "successful" << endl; // Insertion was successful.
                }
                else {
                    cout << "unsuccessful" << endl; // Node with this UFID already exists, insertion unsuccessful.
                }
            }
            else {
                cout << "unsuccessful" << endl; // UFID or name did not pass validation, insertion unsuccessful.
            }
        }


        else if (action == "remove") {
            if (validUFID(details)) {
                if (nodeExists(root, details)) { // Check if the node exists before attempting to remove it.
                    root = deleteHelper(root, details);
                    cout << "successful" << endl;
                }
                else {
                    cout << "unsuccessful" << endl; // Node with this UFID does not exist, deletion unsuccessful.
                }
            }
            else {
                cout << "unsuccessful" << endl; // UFID validation failed, deletion unsuccessful.
            }
        }

        else if (action == "printInorder") {
            vector<Node*> nodes;
            inOrder(root, nodes);
            for (int i = 0; i < nodes.size(); i++) {
                cout << nodes[i]->name;
                if (i < nodes.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }

        else if (action == "printPreorder") {
            vector<Node*> nodes;
            preOrder(root, nodes);
            for (int i = 0; i < nodes.size(); i++) {
                cout << nodes[i]->name;
                if (i < nodes.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }

        else if (action == "printPostorder") {
            vector<Node*> nodes;
            postOrder(root, nodes);
            for (int i = 0; i < nodes.size(); i++) {
                cout << nodes[i]->name;
                if (i < nodes.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }

        else if (action == "printLevelCount") {
            cout << getHeight(root) << endl;
        }

        else if (action == "removeInorder") {
            int n = stoi(details);
            if (n > 0 && countNodes(root) >= n) {
                root = removeNthInorderHelper(root, n);
                if (root) {
                    cout << "successful" << endl;
                }
                else {
                    cout << "unsuccessful" << endl;
                }
            }
            else {
                cout << "unsuccessful" << endl;
            }
        }

        else if (action == "search") {
            if (details.front() == '\"' && details.back() == '\"') {
                string name = details.substr(1, details.size() - 2);
                if (validName(name)) {
                    searchByName(name);
                }
                else {
                    cout << "unsuccessful" << endl;
                }
            }
            else {
                if (validUFID(details)) {
                    searchByID(details);
                }
                else {
                    cout << "unsuccessful" << endl;
                }
            }
        }

        /*else {
            cout << "unsuccessful" << endl;
        }*/
    }


    AVLTree(): root(nullptr){}
    ~AVLTree(){
        treeDestructor(root);
    }


};
