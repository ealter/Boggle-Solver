#include "trie.h"
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h> //for debugging
#include <stdint.h>

#define T trieNodeIdentifier

typedef struct trieNode {
  T nodes[26];
  T parent;
  char c; /* The letter associated with this node. If parent = null, then c
             should be the null terminator. */
  bool isWord;
} trieNode;

//Use a static array of trieNodes so that we don't have to call malloc a lot
#define INITIAL_NODES_ARRAY_SIZE 1024 //Should be a multiple of 32
static trieNode *nodes = NULL;   //The 0th element is unused.
static uint32_t *marks = NULL;   //A parallel array that says whether a node is marked
#define MARK_BITS (8 * sizeof(*marks))
static unsigned nodesLength = 0; //The number of used elements in the nodes array
static unsigned nodesSize   = 0; //The size of the nodes array

#define NULL_TRIE 0

static inline trieNode *getNode(T n) {
  return nodes + n;
}

T trieNode_new()
{
  if(!nodes) {
    nodesSize = INITIAL_NODES_ARRAY_SIZE;
    nodes = calloc(nodesSize, sizeof(*nodes));
    marks = calloc(nodesSize/MARK_BITS, sizeof(*marks));
    assert(nodes && marks);
    nodesLength++; //Don't use the 0th element
  }
  if(nodesLength >= nodesSize) {
    nodesSize *= 2;
    nodes = realloc(nodes, nodesSize * sizeof(*nodes));
    assert(nodes);
    memset(nodes + nodesLength, 0, sizeof(*nodes) * (nodesSize - nodesLength));
    marks = realloc(marks, nodesSize * sizeof(*marks) / MARK_BITS);
    assert(marks);
    memset(marks + nodesLength/MARK_BITS, 0,
          (nodesSize - nodesLength) * sizeof(*marks) / MARK_BITS);
  }

  return nodesLength++;
}

T trieNode_at(T trieNum, char letter)
{
  if(letter < 'a' || letter > 'z')
    return NULL_TRIE;
  assert(trieNum);
  assert(trieNum < nodesLength);
  trieNode *trie = getNode(trieNum);
  return trie->nodes[letter - 'a'];
}

void trieNode_add(T trieNum, char *word)
{
  char c;
  assert(trieNum < nodesLength);
  trieNode *trie = getNode(trieNum);
  assert(trie);
  for(;islower(c = *word); word++) {
    trie = getNode(trieNum);
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
  if(c == '\n' || c == '\0')
    getNode(trieNum)->isWord = true;
}

char *trieNode_toString(T trieNum)
{
  if(!trieNum)
    return NULL;
  /* Get the length of the string */
  int length = -1;
  assert(trieNum < nodesLength);
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
  assert(trie < nodesLength);
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

void trieNode_mark(T trieNum)
{
  assert(trieNum && trieNum < nodesLength);
  marks[trieNum/MARK_BITS] |= (1 << (trieNum % MARK_BITS));
}

bool trieNode_isMarked(T trieNum)
{
  assert(trieNum && trieNum < nodesLength);
  return marks[trieNum/MARK_BITS] & (1 << (trieNum % MARK_BITS));
}

void trieNode_clearAllMarks()
{
  memset(marks, 0, nodesLength * sizeof(*marks)/MARK_BITS);
}

