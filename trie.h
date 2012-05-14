#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED 

#include <stdbool.h>

#define T trieNode
typedef struct T T;

extern T*   trieNode_new();
/* Warning, trieNode_put can cause memory leaks if the parent already had a
 * child for that letter. */
extern T*   trieNode_put(T *parent, T *child, char letter); //TODO: delete
extern T*   trieNode_at(T *trie, char letter); //Precondition: 'a' <= letter <= 'z'
extern void trieNode_add(T *trie, char *word);
extern bool trieNode_isWord(T *trie);
extern void trieNode_free(T *trie);

#undef T
#endif
