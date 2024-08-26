//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Hanna Parham
// Version     : 1.0
// Copyright   : Copyright © 2023 SNHU COCE
// Description : Project Two Code
//============================================================================

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

using namespace std;

// Definition of the TreeNode structure

struct TreeNode {
    string courseId;
    string courseTitle;
    vector<string> prerequisites;
    TreeNode* left;
    TreeNode* right;
    
    TreeNode(string id, string title)
        : courseId(id), courseTitle(title), left(nullptr), right(nullptr) {}
};

// Function to insert a new node into the BST

TreeNode* Insert(TreeNode* root, const string& id, const string& title, const vector<string>& prerequisites) {
    if (root == nullptr) { // If tree or subtree is empty, create new node and return it
        TreeNode* newNode = new TreeNode(id, title);
        newNode->prerequisites = prerequisites;
        return newNode;
    }
    
    if (id < root->courseId) { // Recursively insert into the left subtree if the new course Id is less than the root's course Id
        root->left = Insert(root->left, id, title, prerequisites);
    } else if (id > root->courseId) {    // Recursively insert into the right subtree if the new course Id is greater than the root's course Id
        root->right = Insert(root->right, id, title, prerequisites);
    }
    
    return root; // Returns root
}

// Function to perform an in-order traversal of the tree

void InOrderTraversal(TreeNode* node, vector<TreeNode*>& sortedCoursesList) {
    if (node == nullptr) return;

    InOrderTraversal(node->left, sortedCoursesList);    // Traverse the left subtree

    sortedCoursesList.push_back(node);    // Visit the current node

    InOrderTraversal(node->right, sortedCoursesList);    // Traverse the right subtree
}

// Function to print all courses in sorted order

void PrintSortedCourses(TreeNode* root) {
    vector<TreeNode*> sortedCoursesList;

    InOrderTraversal(root, sortedCoursesList);  // Perform in-order traversal to get courses in sorted order
    
    // Prints each course's details

    for (const auto& courseNode : sortedCoursesList) {
        cout << courseNode->courseId << ": " << courseNode->courseTitle << endl;
        if (!courseNode->prerequisites.empty()) { // Checks if course has prereqs
            cout << "Prerequisites: ";
            for (size_t i = 0; i < courseNode->prerequisites.size(); ++i) { // Iterate over prereq list
                cout << courseNode->prerequisites[i];
                if (i != courseNode->prerequisites.size() - 1) {
                    cout << ", "; // Comma for separation if needeed
                }
            }
            cout << endl;
        }
    }
}

// Function to find a course by course Id

TreeNode* Find(TreeNode* root, const string& id) {
    if (root == nullptr || root->courseId == id) {
        return root;
    }
    
    if (id < root->courseId) {
        return Find(root->left, id); // if Id is less than current node for course Id, search in left
    } else { // if Id is greater than current node for course Id, search in right
        return Find(root->right, id);
    }
}

// Function to load course data from file and insert into BST

void LoadCourseData(TreeNode*& root) {
    string filename;
    cout << "Enter the filename containing course data (ie. CS 300 ABCU_Advising_Program_Input.csv): ";
    cin.ignore();  // Ignore any newline characters in input
    getline(cin, filename);

    ifstream file(filename);
    if (!file.is_open()) {
        cout << "[ERROR] Failed to open file: " << filename << endl; // Error message for file not opening.
        return;
    }

    string line;
    string currentId;
    while (getline(file, line)) {
        // Check if the line is a course Id
        if (!line.empty() && isupper(line[0])) {
            currentId = line;
            string courseTitle;
            if (!getline(file, courseTitle)) break;
            vector<string> prerequisites;
            prerequisites.push_back(courseTitle);

            // Reads prerequisites until the next course Id or the end of file
            while (getline(file, line) && !line.empty() && !isupper(line[0])) {
                prerequisites.push_back(line);
            }

            root = Insert(root, currentId, courseTitle, prerequisites);

            // Line to process new iteration if needed
            if (!line.empty() && isupper(line[0])) {
                file.seekg(-static_cast<int>(line.length() + 1), ios_base::cur);
            }
        }
    }

    file.close();
    cout << "Course data loaded successfully from " << filename << "." << endl; // Tells user the file is successfully loaded
}

// Function to print Course Details

void PrintCourseDetails(TreeNode* root, const string& courseId) {
    TreeNode* node = Find(root, courseId); // Finds course Id, then prereqs
    if (node != nullptr) {
        cout << "Course: " << node->courseId << " - " << node->courseTitle << endl;
        if (!node->prerequisites.empty()) {
            cout << "Prerequisites: "; // prints out prerequisites
            for (size_t i = 1; i < node->prerequisites.size(); ++i) {
                cout << node->prerequisites[i];
                if (i != node->prerequisites.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
    } else {
        cout << "Course not found." << endl; // Error message
    }
}

int main() { // Function for Menu loop + Input
    int choice;
    TreeNode* root = nullptr;

    do {
        // Prints menu options
        cout << "\n     Menu Options:\n";
        cout << "\n";
        cout << "1. Load Course Data\n";
        cout << "2. Print All Courses (Alphanumerical Order)\n";
        cout << "3. Print Course Title + Prerequisites\n";
        cout << "9. Exit the Program\n";
        cout << "\nPlease enter your choice: ";
        
        cin >> choice; // Enter choice

        switch (choice) {
            case 1:
                LoadCourseData(root);
                break;
            case 2:
                PrintSortedCourses(root);  // Print courses in alphanumerical order
                break;
            case 3: {
                string courseId;
                cout << "Enter Course ID: "; // Input course Id
                cin >> courseId;
                PrintCourseDetails(root, courseId);
                break;
            }
            case 9:
                cout << "Exiting the program..." << endl; // Closes program when picked
                break;
            default:
                cout << "Invalid choice. Please enter a valid numerical option from the menu." << endl; // Error message for choice number not on the menu
        }
    } while (choice != 9);

    return 0;
}