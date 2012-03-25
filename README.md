#About
Simple implementation of the <a href="http://en.wikipedia.org/wiki/Huffman_coding">Huffman algorithm</a> for the purpose of developing my C skills.


#Implementation details
- Byte buffers (dynamic arrays of bytes; see *buffer.h*) are used for storing data during compression and decoding.
- Nodes (see *node.h*) are used to represent the Huffman tree and store characters in the case of leaf nodes.
- Codes (see *codes.h*) are used to represent the Huffman codes for each character.
- A simple profiler (see *profiler.h*) is used to time certain portions of program execution to aid in optimization and to enable performance comparisons with other implementations.


#Notable files
- *huffman.c* the actual implementation of the algorithm
- *huffman_test.c* example usage of the API
