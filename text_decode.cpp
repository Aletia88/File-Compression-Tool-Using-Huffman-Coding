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
        if (_mkdir(folder_path) == 0) {
            cout << "Folder created: " << folder_path << endl;
            return 0;
        } else {
            cout << "folder already exists: " << folder_path << endl;
            return 0;  // Changed to -1 for failure
        }
    #else
        if (mkdir(folder_path, 0777) == 0) {
            cout << "Folder created: " << folder_path << endl;
            return 0;
        } else {
            cout << "Failed to create folder or folder already exists: " << folder_path << endl;
            return -1;  // Changed to -1 for failure
        }
    #endif
}

unsigned int read_header(FILE* fr) {
    unsigned int buffer;
    fread(&buffer, sizeof(buffer), 1, fr);
    return buffer;
}

Tree_node* rebuild_huffman_tree(FILE* fr) {
    struct Tree_node* left, * right, * top;
    stack<Tree_node*> stack;
    unsigned int header_len = read_header(fr);
    unsigned int map_len = read_header(fr);
    cout << "The header has total length of " << header_len << endl;
    int idx_header = 0;
    unsigned char str;
    unsigned int num;
    string tmp = "";
    string bin_num;
    bool flag = true;
    while (idx_header < header_len) {
        fread(&str, sizeof(str), 1, fr);
        num = int(str);
        bin_num = int_to_binary(num);
        int idx_loop = 0;
        while (idx_loop < bin_num.size()) {
            if (tmp.size() == 0 && flag == true) {
                if (bin_num[idx_loop] == '1') {
                    flag = false;
                } else {
                    if (stack.size() == 1) {
                        break;
                    }
                    Tree_node* right_node = stack.top();
                    stack.pop();
                    Tree_node* left_node = stack.top();
                    stack.pop();
                    Tree_node* new_node = new Tree_node('#');
                    new_node->left = left_node;
                    new_node->right = right_node;
                    stack.push(new_node);
                    if (map_len == 0) {
                        flag = true;
                    }
                }
            } else {
                tmp += bin_num[idx_loop];
            }
            if (tmp.size() == 8) {
                char new_char = binary_to_char(tmp);
                Tree_node* new_node = new Tree_node(new_char);
                stack.push(new_node);
                tmp = "";
                map_len -= 1;
                flag = true;
            }
            idx_loop += 1;
        }
        idx_header += 1;
    }
    return stack.top();
}

unordered_map<string, char> build_decoding_map(Tree_node* root, string str, unordered_map<string, char> map, unsigned int file_len) {
    if (!root) {
        return map;
    }
    if (root->data != '#') {
        map[str] = root->data;
    }
    map = build_decoding_map(root->left, str + "0", map, file_len);
    map = build_decoding_map(root->right, str + "1", map, file_len);
    return map;
}

void decoding(FILE* fr, FILE* fw, unordered_map<string, char> map, int file_len) {
    string tmp = "";
    int idx;
    unsigned char str;
    int num;
    string bin_num;
    int current_length = 0;
    while (!feof(fr)) {
        idx = 0;
        fread(&str, sizeof(str), 1, fr);
        num = int(str);
        bin_num = int_to_binary(num);
        while (idx < bin_num.size()) {
            if (current_length == file_len) {
                break;
            }
            tmp += bin_num[idx];
            idx += 1;
            if (map.find(tmp) != map.end()) {
                unsigned char buff = map[tmp];
                fwrite(&buff, sizeof(buff), 1, fw);
                cout << "Written byte: " << buff << endl;  // Debugging line
                tmp = "";
                current_length += 1;
            }
        }
    }
    cout << "Decoding successfully!" << endl;
}

int main() {
    const char* decoded_folder = "decoded_files";
    int folder_result = create_folder(decoded_folder);
    if (folder_result == -1) {
        cerr << "Error creating folder. Exiting program." << endl;
        return 0;
    }

    const char* input_file = "encoded_files/test_output_encoded.txt";
    const char* output_file = "decoded_files/test_output_decoded.txt";

    FILE* input = fopen(input_file, "rb");
    if (!input) {
        cerr << "Failed to open input file: " << input_file << endl;
        return 0;
    }

    unsigned int file_len = read_header(input);
    Tree_node* top = rebuild_huffman_tree(input);
    unordered_map<string, char> decoding_map;
    decoding_map = build_decoding_map(top, "", decoding_map, file_len);

    FILE* output = fopen(output_file, "wb");  // Open in binary mode for writing
    if (!output) {
        cerr << "Failed to open output file: " << output_file << endl;
        return 0;
    } else {
        cout << "Output file opened successfully: " << output_file << endl;
    }

    decoding(input, output, decoding_map, file_len);

    fclose(input);
    fclose(output);

    cout << "Decoding completed successfully!" << endl;

    return 0;
}
