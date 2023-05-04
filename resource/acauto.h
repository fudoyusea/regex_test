#include <iostream>
#include <string>

typedef struct trie_node_t {
    uint8_t ch;
    trie_node_t * fail;
    bool tail_node;
    trie_node_t * next[];
};