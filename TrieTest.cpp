#include "Trie.h"

int main() {
    Trie<char> dictionary;
    dictionary.insert({'a', 'p', 'p', 'l', 'e'});
    dictionary.insert({'a', 'p', 'p'});
    dictionary.insert({'b', 'a', 'n', 'a', 'n', 'a'});
}