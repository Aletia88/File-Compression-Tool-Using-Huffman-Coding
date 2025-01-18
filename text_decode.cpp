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
using namespace std;
// Comment 1: This struct represents a node in the Huffman tree used for decoding
// Comment 2: Each node contains a character, and pointers to left and right child nodes

struct Tree_node 
//tree node for rebuilding huffman tree
{
	char data;
	Tree_node* left;
	Tree_node* right;
	Tree_node(char data) {
		this->left = NULL;
		this->right = NULL;
		this->data = data;
	}

};

unsigned int read_header(FILE* fr) {
	// helper function to read first three unsigned int
	unsigned int buffer;
	fread(&buffer,sizeof(buffer),1,fr);
	return buffer;
}


Tree_node* rebuild_huffman_tree(FILE* fr) {
	// we will use a stack to rebuild our huffman tree
	// when we see a "1", it represents a leaf node, 
	// we will create a new node and push to the stack with following char value
	// " 0" denotes the non-leaf node, we will pop the first two nodes and merge them
	struct Tree_node* left, * right,* top;
	stack<Tree_node*> stack;
	unsigned int header_len= read_header(fr);
	unsigned int map_len= read_header(fr);
	cout << " the header has total length of "<<header_len<<endl;
	int idx_header = 0;
	unsigned char str;
	unsigned int num;
	string tmp = "";
	string bin_num;
	bool flag = true;
	while (idx_header<header_len){
		fread(&str,sizeof(str),1,fr);
		num = int(str);
		bin_num = int_to_binary(num);
		// cout << " now read binary "<<bin_num<<endl;
		int idx_loop = 0;
		while (idx_loop < bin_num.size()){
			if (tmp.size() == 0 and flag == true){
				if (bin_num[idx_loop] == '1'){
					flag = false;
				}
				else{
					// current is 0 which is  a non_leaf node
					if (stack.size()==1){
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
					if (map_len == 0){
						flag = true;
					}
					}
					
			}
			else{
				tmp += bin_num[idx_loop];}
			if (tmp.size() == 8){
				char new_char = binary_to_char(tmp);
				// cout << " now push new tree node "<<new_char<<endl;
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

unordered_map<string,char> build_decoding_map(Tree_node* root,string str,unordered_map<string, char> map,unsigned int file_len){
	// tranverse our huffman tree in post order to build our decoding map 
	if (!root) {
		return map;
	}
	if (root->data != '#') {
		map[str] = root->data;
	}
	map = build_decoding_map(root->left, str + "0",map,file_len);
	map = build_decoding_map(root->right, str + "1",map,file_len);
	return map;
}

void decoding(FILE* fr, FILE* fw,unordered_map<string, char> map,int file_len) {
	// read a single bit at a time and use our previously built huffman map to encode
	string tmp = "";
	int idx;
	unsigned char str;
	int num;
	string bin_num;
	int current_length = 0;
	while (!feof(fr)){
		idx = 0;
		fread(&str,sizeof(str),1,fr);
		num = int(str);
		bin_num = int_to_binary(num);
		while (idx < bin_num.size()){
			if (current_length == file_len){
				break;
			}
			tmp += bin_num[idx];
			idx += 1;
			if (map.find(tmp) != map.end()){
				unsigned char buff = map[tmp];
				// cout << "now write char "<<buff<<endl;
				fwrite(&buff,sizeof(buff),1,fw);
				tmp = "";
				current_length += 1;
			}
		}
		
	}
	cout <<"decoding successfully!"<<endl;
}



int main() {
	// open the input and output file
	const char* input_file = "test_output_encoded.txt";
	const char* output_file= "test_output_decoded.txt";
	FILE* input = fopen(input_file, "rb");
	int result = open_file(input);
	if (result == -1){
		return 0;
    }
	unsigned int file_len = read_header(input);
	Tree_node* top;
	top = rebuild_huffman_tree(input);
	unordered_map<string, char> decoding_map;
	decoding_map = build_decoding_map(top,"",decoding_map,file_len);
	FILE* output = fopen(output_file, "w+");
	result = open_file(output);
	if (result == -1){
		return 0;
    }
	decoding(input,output,decoding_map,file_len);
	fclose(input);
	fclose(output);
	return 0;
}
