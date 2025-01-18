#include <iostream>
#include <cmath>
#include <vector>
#include <unordered_map>
#include <string>
#include <queue>
#include <fstream>
#define _CRT_SECURE_NO_WARNINGS
using namespace std;

struct minheapnode 
//minheap node for building huffman heap
{
	unsigned freq;
	char data;
	minheapnode* left;
	minheapnode* right;
	minheapnode(char data, int freq) {
		this->left = NULL;
		this->right = NULL;
		this->data = data;
		this->freq = freq;
	}

};
struct compare {
	bool operator()(minheapnode* a, minheapnode* b) {
		return a->freq > b->freq;
	}
};

unordered_map<char, int> build_frequency_table(FILE* input) {
    // count word frequency from file, and output as a hashmap
	unordered_map<char, int> map;
	char tmp;
	while (true) {
		tmp = fgetc(input);
		if (tmp == EOF) {
			cout << "frequency map build successfully!"<<endl;
			break;
		}
		if (map.find(tmp) != map.end()) {
			map[tmp] += 1;
		}
		else {
			map[tmp] = 1;
		}
	}
	return map;
}


priority_queue<minheapnode*, vector<minheapnode*>, compare> build_huffman_heap(unordered_map<char, int> map) {
    // build huffman heap by using a min heap
	struct minheapnode* left, * right, * top;
	priority_queue<minheapnode*, vector<minheapnode*>, compare> minheap;
	unordered_map<char, int>::iterator itr;
	for (itr = map.begin(); itr != map.end();itr++) {
		char key = itr->first;
		int val = itr->second;
		minheapnode* node = new minheapnode(key, val);
		minheap.push(node);
	}
	while (minheap.size() != 1) {
		minheapnode* node1 = minheap.top();
		minheap.pop();
		minheapnode* node2 = minheap.top();
		minheap.pop();
		minheapnode* new_node = new minheapnode('#', node1->freq + node2->freq);
		new_node->left = node1;
		new_node->right = node2;
		minheap.push(new_node);
	}
	return minheap;

	
}

int get_header_length(int len, minheapnode* root) {
    // we could build a header to store our huffman tree
	if (!root) {
		return 0;
	}
	int left, right, current;
	left = get_header_length(0, root->left);
	right = get_header_length(0, root->right);
	if (root->data != '#') {
		current = 1;
	}
	else {
		current = 0;
	}
	return left + right + current;
}

void print_huffman_tree(minheapnode*root,string str) {
    // use for debugging and print the huffman tree in order
	if (!root) {
		return;
	}
	if (root->data != '#') {
		cout << " the string " <<" "<< root->data << " with frequency " << root->freq << " has huffman code " << str << endl;
	}
	print_huffman_tree(root->left, str + "0");
	print_huffman_tree(root->right, str + '1');
}

unordered_map<char, string> build_encoding_map(minheapnode* root, string str, unordered_map<char, string> encoding_map) {
    // build hash map
	if (!root) {
		return encoding_map;
	}
	if (root->data != '#') {
		encoding_map[root->data] = str;
	}
	encoding_map = build_encoding_map(root->left, str + "0",encoding_map);
	encoding_map = build_encoding_map(root->right, str + "1",encoding_map);
	return encoding_map;
}


string compress_huffman_tree(string str,minheapnode* root) {
    // compress the huffman tree by post order so that we can store it in our header
	string left = "";
	string right = "";
	string current = "";
	if (!root) {
		return "";
	}
	left = compress_huffman_tree(str, root->left);
	right = compress_huffman_tree(str, root->right);
	if (root->data != '#') {
		current = "1";
		current += root->data;
	}
	else {
		current += "0";
	}
    // cout << str + left + right + current;
    // cout<<"---"<<endl;
	return str + left + right + current;

}

string int_to_binary(int num) {
    // helper function to tranform integer to binary number in string format
	string str = "00000000";
	int idx = 0;
	while (num > 0) {
		if (num % 2 != 0) {
			str[idx] = '1';
		}
		idx++;
		num = num / 2;
	}
	return str;


}
unsigned char binary_to_char(string str) {
    // helper function to transform binary number to char by using ascii code
	int tmp = 0;
	for (int i = 0; i < 8;i++) {
		if (str[i] == '1') {
			tmp += pow(2, i);
		}
	}
	unsigned char ans = char(tmp);
	return ans;
}


void write_header(FILE* fw,string str) {
    // write the compressed huffman tree to our file
	string tmp = "00000000";
	int idx = 0;
	unsigned char buff;
	int ascii_value;
    	bool flag = true;
	string bin;
	for (int i = 0;i < str.size();i++) {
        if (flag == true) {
            tmp[idx] = str[i];
            if (tmp[idx] == '1'){
                flag = false;
            }
            idx+=1;
        }
        else {
            ascii_value = int(str[i]);
            bin = int_to_binary(ascii_value);
            for (int j = 0; j < tmp.size();j++) {
                tmp[idx] = bin[j];
                idx+=1;
                if (idx == 8) {
                    idx = 0;
                    buff = binary_to_char(tmp);
                    // cout << "now write binary number to file "<< tmp << endl;
                    tmp = "00000000";
                    fwrite(&buff, sizeof(unsigned char), 1, fw);
                }
            }
            flag = true;

        }

        if (idx == 8) {
            idx = 0;
            buff = binary_to_char(tmp);
            fwrite(&buff, sizeof(unsigned char), 1, fw);
            // cout << "now write binary number to file "<< tmp << endl;
            tmp = "00000000";
            }

	}
	if (idx != 0) {
		for (int pos = idx;pos < tmp.size();pos++) {
			tmp[pos] = '0';
		}
		buff = binary_to_char(tmp);
        // cout << "now write binary number to file "<< tmp << endl;
		fwrite(&buff, sizeof(unsigned char), 1, fw);
	}
	return;
    }

int open_file(FILE* input){
	if (input == NULL) {
		cout << "File did not open successfully, please check " << endl;
		return -1;
	}
	else {
		cout << "File opened successfully!" << endl;
        return 0;
	}
}

int count_length(FILE* input){
    	// count the length of input file
    	fseek(input, 0L, SEEK_END);
	unsigned int file_len = ftell(input);
	if (fseek(input, 0, SEEK_SET) != 0) {
		cout << "failed to rewind the pointer for input file" << endl;
	return -1;
	}
	else{
	cout << "the input file has total " << file_len << " bytes" << endl;
	return file_len;
	}
}



void encoding(FILE* fr,FILE*fw, unordered_map<char, string>map) {
    // use previous built huffman tree to encode our input file
	char str;
	int idx = 0;
	string tmp = "00000000";
    	fseek(fr, 0, SEEK_SET); //set the pointer to the beginning of our input file
	unsigned char buff;
	string huff_code;
	while (1) {
		str = fgetc(fr);
		if (str == EOF) {
			if (idx != 0) {
				for (int i = idx; i < 8; i++) {
					tmp[i] = 0;
				}
				buff = binary_to_char(tmp);
				fwrite(&buff, sizeof(unsigned char), 1, fw);
			}
			break;
		}
		huff_code = map[str];
        // cout << "encoded string "<<str<<endl;
		for (int i = 0;i < huff_code.size();i++) {
			tmp[idx] = huff_code[i];
			idx += 1;
			if (idx == 8) {
				idx = 0;
				buff = binary_to_char(tmp);
				fwrite(&buff, sizeof(unsigned char), 1, fw);
				// cout << "write successfully";
				tmp = "00000000";
			}
		}
		
	}
	cout << "file compressed successfully" << endl;

}
int main() {
	// we will first store 3 unsigned int to our header, 
    // which are the length of file,length of header and the length of characters
	const char* file_name = "test.txt";
    	FILE* input = fopen(file_name, "rb");
	int result = open_file(input);
	if (result == -1){
		return 0;
	}
	int file_len = count_length(input);
    	if (file_len == -1){
        	return 0;
    	} 
	unordered_map<char, int> frequency_table = build_frequency_table(input);
	priority_queue<minheapnode*, vector<minheapnode*>, compare> huffman_heap = build_huffman_heap(frequency_table);
	unordered_map<char, string> encoding_map;
	encoding_map = build_encoding_map(huffman_heap.top(), "", encoding_map);
    	int map_length = encoding_map.size();
	string compressed_tree = compress_huffman_tree("", huffman_heap.top());
	compressed_tree += '0';
	const char* output_file = "test_output_encoded.txt";
	FILE* output = fopen(output_file, "wb");
	fwrite(&file_len, sizeof(unsigned int), 1, output);
	unsigned int header_len = get_header_length(0, huffman_heap.top());
	header_len = 10 * map_length;
	if (header_len % 8 != 0) {
		header_len = header_len / 8 + 1;
	}
	else {
		header_len = header_len / 8;
	}
	cout << "the header has total " << header_len << " bytes" << endl;
	fwrite(&header_len, sizeof(unsigned int), 1, output);
    	fwrite(&map_length, sizeof(unsigned int), 1, output);
	write_header(output,compressed_tree);
	encoding(input, output, encoding_map);
	fclose(input);
	fclose(output);
	return 0;
}