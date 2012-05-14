#include "trie.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h> //for debugging

#define T trieNode
struct T {
  T* nodes[26];
  bool isWord;
};

T* trieNode_new()
{
  T* trie = (T*)calloc(1, sizeof(*trie));
  return trie;
}

T* trieNode_put(T *parent, T *child, char letter)
{
  assert(parent);
  assert(islower(letter));
  parent->nodes[letter - 'a'] = child;
  return child;
}

T* trieNode_at(T* trie, char letter)
{
  letter = tolower(letter);
  if(letter < 'a' || letter > 'z')
    return NULL;
  assert(trie);
  return trie->nodes[letter - 'a'];
}
  
void trieNode_add(T *trie, char *word)
{
  char c;
  for(;(c = *word) != '\0' && c != '\n'; word++) {
    trieNode *node = trieNode_at(trie, c);
    if(!node) {
      if(!islower(c))
        return;
      node = trieNode_new();
      trie->nodes[c - 'a'] = node;
    }
    trie = node;
  }
  trie->isWord = true;
}

bool trieNode_isWord(T *trie)
{
  assert(trie);
  return trie->isWord;
}

void trieNode_free(T *trie)
{
  assert(trie);
  for(int i=0; i<26; i++) {
    if(trie->nodes[i])
      trieNode_free(trie->nodes[i]);
  }
  free(trie);
}
