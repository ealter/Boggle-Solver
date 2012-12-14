#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <stdbool.h>

#define T trieNodeIdentifier
typedef unsigned T;

extern T    trieNode_new();
extern T    trieNode_at(T trie, char letter); //Precondition: 'a' <= letter <= 'z'
extern void  trieNode_add(T trie, char *word);
extern char *trieNode_toString(T trie);
extern bool  trieNode_isWord(T trie);
extern void  trieNode_freeAll(); //Frees all trie Nodes

extern void trieNode_mark(T trie);
extern bool trieNode_isMarked(T trie);
extern void trieNode_clearAllMarks();

#undef T
#endif
