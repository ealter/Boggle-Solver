#ifndef BOGGLE_INCLUDED
#define BOGGLE_INCLUDED

#include "trie.h"
#include <stdio.h>

typedef struct wordList {
  unsigned arraySize; /* The size of the words array */
  unsigned numWords;  /* The number of non-null entriees in the words array */
	char **words;  /* The list of words. Invariant: A word is null iff all of the
                    words afterwards are also null */
} wordList;

/* The file is a line separated list of words */
extern trieNode *makeDictionary(FILE *fp);

/* Returns a list of words in the boggle board. letters is an array of
 * characters of the board in row-major order. boardSize is the number of rows
 * for the square board (i.e. for a 4x4 board, boardSize=4)
 * Precondition: 1 <= boardSize <= 8 */
extern wordList solveBoard(trieNode *dict, char *letters, unsigned boardSize);

#endif
