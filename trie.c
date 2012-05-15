#include "trie.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h> //for debugging

#define T trieNode
struct T {
  T* nodes[26];
  T* parent;
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
  if(child)
    child->parent = parent;
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
  assert(trie);
  for(;(c = *word) != '\0' && c != '\n'; word++) {
    if(!islower(c))
      return;
    if(c == 'q') {
      if(*(++word) != 'u')
        return;
    }
    trieNode *node = trie->nodes[c - 'a'];
    if(!node) {
      node = trieNode_new();
      trie->nodes[c - 'a'] = node;
      node->parent = trie;
    }
    trie = node;
  }
  trie->isWord = true;
}

char *trieNode_toString(T *trie)
{
  if(!trie)
    return NULL;
  /* Get the length of the string */
  int length = -1;
  for(T *t = trie; t != NULL; t=t->parent, length++);
  char *str = malloc((length + 1) + sizeof(*str));
  assert(str);
  int i = length - 1;
  for(T *t = trie; i>=0; i--, t=t->parent) {
    char c = ' ';
    /* Find out which character this node is associated with */
    for(int j=0; j<26; j++) {
      if(t->parent->nodes[j] == t) {
        c = j + 'a';
        break;
      }
      assert(j != 26); //make sure a letter was found
    }
    str[i] = c;
  }
  str[length - 1] = '\0';
  return str;
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
