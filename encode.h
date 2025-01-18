#ifndef ENCODING
#define ENCODING
#include<string>
#include<iostream>
#include <cmath>
using namespace std;

struct minheapnode
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
string int_to_binary(int num) {
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

int open_file(FILE* input){
	if (input == NULL) {
		cout << "Files  did not opened successfully, please check again" << endl;
		return -1;
	}
	else {
		cout << "Files are opened successfully!" << endl;
        return 0;
	}
}

unsigned char binary_to_char(string str) {
	int num = 0;
	for (int i = 0; i < 8;i++) {
		if (str[i] == '1') {
			num+= pow(2, i);
		}
	}
	unsigned char ans = char(num);
	return ans;
}
#endif
