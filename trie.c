#include "trie.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h> //for debugging

#define T trieNodeIdentifier

typedef struct trieNode {
  T nodes[26];
  T parent;
  char c; /* The letter associated with this node. If parent = null, then c
             should be the null terminator. */
  bool isWord;
} trieNode;

//Use a static array of trieNodes so that we don't have to call malloc a lot
#define INITIAL_NODES_ARRAY_SIZE 1000
static trieNode *nodes = NULL;   //The 0th element is unused.
static unsigned nodesLength = 0; //The number of used elements in the nodes array
static unsigned nodesSize   = 0; //The size of the nodes array

#define NULL_TRIE 0

static inline trieNode *getNode(T n) {
  assert(n < nodesLength);
  return nodes + n;
}

T trieNode_new()
{
  if(!nodes) {
    nodesSize = INITIAL_NODES_ARRAY_SIZE;
    nodes = calloc(nodesSize, sizeof(*nodes));
    assert(nodes);
    nodesLength++; //Don't use the 0th element
  }
  if(nodesLength >= nodesSize) {
    nodesSize *= 2;
    nodes = realloc(nodes, nodesSize * sizeof(*nodes)); //TODO: this breaks stuff.
    assert(nodes);
    memset(nodes + nodesLength, 0, sizeof(*nodes) * (nodesSize - nodesLength)); //Might have off-by-one
  }

  return nodesLength++;
  //T* trie = calloc(1, sizeof(*trie));
  //assert(trie);
  //return trie;
}

T trieNode_put(T parentNum, T childNum, char letter)
{
  trieNode *parent = getNode(parentNum);
  assert(parent);
  assert(islower(letter));
  parent->nodes[letter - 'a'] = childNum;
  if(childNum) {
    trieNode *child = getNode(childNum);
    child->parent = parentNum;
    child->c = letter;
  }
  return childNum;
}

T trieNode_at(T trieNum, char letter)
{
  if(letter < 'a' || letter > 'z')
    return NULL_TRIE;
  assert(trieNum);
  trieNode *trie = getNode(trieNum);
  return trie->nodes[letter - 'a'];
}

void trieNode_add(T trieNum, char *word)
{
  char c;
  trieNode *trie = getNode(trieNum);
  assert(trie);
  for(;(c = *word) != '\0' && c != '\n'; word++) {
    trie = getNode(trieNum);
    if(!islower(c))
      return;
    if(c == 'q') {
      if(*(++word) != 'u')
        return;
    }
    T nodeNum = trie->nodes[c - 'a'];
    if(!nodeNum) {
      nodeNum = trieNode_new();
      getNode(trieNum)->nodes[c - 'a'] = nodeNum;
      trieNode *node = getNode(nodeNum);
      node->parent = trieNum;
      node->c = c;
    }
    trieNum = nodeNum;
  }
  getNode(trieNum)->isWord = true;
}

char *trieNode_toString(T trieNum)
{
  if(!trieNum)
    return NULL;
  /* Get the length of the string */
  int length = -1;
  for(T t = trieNum; t != NULL_TRIE; t=getNode(t)->parent, length++){}
  char *str = malloc((length + 1) * sizeof(*str));
  assert(str);
  int i = length - 1;
  for(trieNode *t = getNode(trieNum); i>=0; i--, t=getNode(t->parent)) {
    str[i] = t->c;
  }
  str[length] = '\0';
  return str;
}

bool trieNode_isWord(T trie)
{
  assert(trie);
  return getNode(trie)->isWord;
}

void trieNode_freeAll()
{
  if(nodes) {
    free(nodes);
    nodes = NULL;
  }
  nodesLength = 0;
  nodesSize = 0;
}
