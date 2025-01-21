# Huffman Decoding Program

## Overview
This C++ program implements a Huffman decoding algorithm. It takes an encoded file, reconstructs the Huffman tree used for encoding, and decodes the file back into its original content. The process includes reading headers, rebuilding the Huffman tree, constructing a decoding map, and finally performing the decoding.

## Description

### Libraries Included
- `<iostream>`: Standard input/output stream for displaying information to the console.
- `<cmath>`: For mathematical operations (not directly used in the code).
- `<vector>`: For using dynamic arrays (not directly used in the code).
- `<unordered_map>`: Provides hash-based maps for storing the decoding table.
- `<string>`: Used for handling strings.
- `<queue>`: Not directly used in the code.
- `<fstream>`: For file I/O operations.
- `<stack>`: Used for handling the stack during Huffman tree reconstruction.

### Struct `Tree_node`
A tree node that is used to represent nodes in the Huffman tree. Each node can either be a leaf node (containing a character) or an internal node (represented by `#`). The `Tree_node` struct contains:
- `char data`: Character in the leaf node or `#` for internal nodes.
- `Tree_node* left`: Pointer to the left child node.
- `Tree_node* right`: Pointer to the right child node.

### Key Functions

1. **`read_header(FILE* fr)`**
   - Reads the header information from the file and returns the header length as an unsigned integer.

2. **`rebuild_huffman_tree(FILE* fr)`**
   - Rebuilds the Huffman tree from the file.
   - Uses a stack to manage nodes as the tree is reconstructed.
   - A `1` in the input stream indicates a leaf node, while a `0` represents a non-leaf node.
   - It processes the header and data to reconstruct the tree structure and returns the root node of the tree.

3. **`build_decoding_map(Tree_node* root, string str, unordered_map<string, char> map, unsigned int file_len)`**
   - Recursively traverses the reconstructed Huffman tree to build a map of binary strings (representing the paths from the root to each leaf) to characters.
   - The result is a map where the key is a binary string and the value is the corresponding character.

4. **`decoding(FILE* fr, FILE* fw, unordered_map<string, char> map, int file_len)`**
   - Reads the encoded file bit-by-bit, uses the decoding map to find corresponding characters, and writes the decoded characters to the output file.
   - Stops when the total decoded length reaches the specified file length.

5. **`main()`**
   - Main entry point of the program.
   - Opens the input and output files.
   - Reads the header information, rebuilds the Huffman tree, constructs the decoding map, and finally decodes the file.

### Program Flow
1. **Open Input and Output Files**: The program opens the input encoded file and output decoded file.
2. **Read File Header**: The header, which contains information about the encoded file, is read.
3. **Rebuild Huffman Tree**: The Huffman tree used during encoding is reconstructed from the file data.
4. **Build Decoding Map**: A map is created that maps binary strings to characters by traversing the reconstructed Huffman tree.
5. **Decode the File**: The encoded file is read bit by bit, using the decoding map to output the decoded characters to the output file.

### Error Handling
- If any file cannot be opened (either for reading or writing), the program will return early with an error code.
  
## Example Usage

1. Input file (`test_output_encoded.txt`): A file that has been encoded using Huffman coding.
2. Output file (`test_output_decoded.txt`): The resulting file after decoding the input file back to its original content.

### Notes
- The program assumes the input file has been encoded using a specific Huffman tree structure, which is reconstructed during the decoding process.
- The program performs decoding bit by bit and writes the decoded characters to the output file when they are found in the decoding map.

## Group Memeber
| #  | NAME                   | ID            |
|----|------------------------|---------------|
| 1  | Michael Tesfaye          | NSR/1678/13   |
| 2  | Tamagn zewdu             | NSR/2229/13   |
| 3  | Emmanuel hirpessa        | NSR/868/13   |
| 4  | Samrawit girma           | NSR/2020/13   |
| 5  | Yohanes dejene           | NSR/2539/13    |


