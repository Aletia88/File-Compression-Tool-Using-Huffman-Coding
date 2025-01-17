#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <fstream>
#include <string>
#include "encode.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stack>
#include <sys/stat.h>  // For mkdir on POSIX systems
#include <direct.h>    // For mkdir on Windows

using namespace std;

struct Tree_node {
    // tree node for building huffman tree
    char data;
    Tree_node* left;
    Tree_node* right;
    Tree_node(char data) {
        this->left = NULL;
        this->right = NULL;
        this->data = data;
    }
};

int create_folder(const char* folder_path) {
    // Attempt to create the directory (Windows or POSIX systems)
    #ifdef _WIN32
        // On Windows, use _mkdir
        if (_mkdir(folder_path) == 0) {
            cout << "Folder created: " << folder_path << endl;
            return 0;
        } else {
            cout << " folder already exists: " << folder_path << endl;
            return 0;
        }
    #else
        // On POSIX (Linux/Mac), use mkdir
        if (mkdir(folder_path, 0777) == 0) {
            cout << "Folder created: " << folder_path << endl;
            return 0;
        } else {
            cout << "Failed to create folder or folder already exists: " << folder_path << endl;
            return -1;
        }
    #endif
}

unordered_map<char, int> build_frequency_table(FILE* fr) {
    unordered_map<char, int> freq_table;
    unsigned char ch;
    while (fread(&ch, sizeof(ch), 1, fr)) {
        freq_table[ch]++;
    }
    return freq_table;
}

struct Compare {
    bool operator()(Tree_node* left, Tree_node* right) {
        return left->data > right->data;  // Min heap
    }
};

Tree_node* build_huffman_tree(unordered_map<char, int> freq_table) {
    priority_queue<Tree_node*, vector<Tree_node*>, Compare> min_heap;
    
    // Create leaf nodes and add them to the priority queue
    for (const auto& pair : freq_table) {
        min_heap.push(new Tree_node(pair.first));
    }

    // Build the Huffman Tree
    while (min_heap.size() > 1) {
        Tree_node* left = min_heap.top(); min_heap.pop();
        Tree_node* right = min_heap.top(); min_heap.pop();

        Tree_node* new_node = new Tree_node('#');
        new_node->left = left;
        new_node->right = right;
        min_heap.push(new_node);
    }

    return min_heap.top();  // Return root of the Huffman tree
}

void build_encoding_map(Tree_node* root, string str, unordered_map<char, string>& encoding_map) {
    if (!root) return;

    // If it's a leaf node, store the encoding
    if (root->data != '#') {
        encoding_map[root->data] = str;
    }

    build_encoding_map(root->left, str + "0", encoding_map);
    build_encoding_map(root->right, str + "1", encoding_map);
}

void encode(FILE* fr, FILE* fw, unordered_map<char, string> encoding_map) {
    unsigned char ch;
    string encoded_str;
    while (fread(&ch, sizeof(ch), 1, fr)) {
        encoded_str += encoding_map[ch];
    }

    // Write the encoded string to the output file
    while (encoded_str.size() >= 8) {
        string byte_str = encoded_str.substr(0, 8);
        unsigned char byte = binary_to_char(byte_str);
        fwrite(&byte, sizeof(byte), 1, fw);
        encoded_str = encoded_str.substr(8);
    }

    // Handle remaining bits (less than 8)
    if (encoded_str.size() > 0) {
        encoded_str = encoded_str + string(8 - encoded_str.size(), '0');
        unsigned char byte = binary_to_char(encoded_str);
        fwrite(&byte, sizeof(byte), 1, fw);
    }

    cout << "Encoding successfully!" << endl;
}

int main() {
    // Create the encoded_files folder if it doesn't exist
    const char* encoded_folder = "encoded_files";
    int folder_result = create_folder(encoded_folder);
    if (folder_result == -1) {
        return 0;  // Exit if the folder couldn't be created or already exists
    }

    // Define the input and output file paths
    const char* input_file = "test.txt";
    const char* output_file = "encoded_files/test_output_encoded.txt";  // Saving to encoded_files folder

    // Open the input file
    FILE* input = fopen(input_file, "rb");
    if (!input) {
        cout << "Error opening input file!" << endl;
        return -1;
    }

    // Build the frequency table and Huffman tree
    unordered_map<char, int> freq_table = build_frequency_table(input);
    Tree_node* root = build_huffman_tree(freq_table);

    // Build the encoding map
    unordered_map<char, string> encoding_map;
    build_encoding_map(root, "", encoding_map);

    // Reopen the input file to reset the file pointer
    fclose(input);
    input = fopen(input_file, "rb");

    // Create the output file in the encoded_files folder
    FILE* output = fopen(output_file, "wb");
    if (!output) {
        cout << "Error opening output file!" << endl;
        return -1;
    }

    // Perform the encoding
    encode(input, output, encoding_map);

    // Close the files
    fclose(input);
    fclose(output);

    cout << "Encoding completed successfully!" << endl;

    return 0;
}
