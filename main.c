#include "boggle.h"
#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

static inline void freeWordList(wordList words)
{
  for(unsigned i=0; i<words.numWords; i++)
    free(words.words[i]);
  free(words.words);
}

int main(int argc, char *argv[])
{
  if(argc != 2) {
    printf("Usage: %s <letters>\n", argv[0]);
    return 1;
  }
  trieNode *dict = makeDictionary(stdin);
  assert(dict);
  char *letters = argv[1];
  removeQUs(letters);
  int numLetters = strlen(letters);
  int boardSize = sqrt(numLetters);
  unsigned minWordLength = boardSize <= 4 ? 3 : 4;
  assert(boardSize * boardSize == numLetters);
  wordList words = solveBoard(dict, letters, boardSize);
  //trieNode_free(dict); //Uncomment to stop leaking memory
  printf("There are %d words\n", words.numWords);
  assert(words.words);
  for(unsigned i=0; i<words.numWords; i++) {
    char *word = words.words[i];
    if(strlen(word) >= minWordLength) {
      char c;
      for(;(c = *word) != '\0'; word++) {
        putchar(c);
        if(c == 'q') putchar('u');
      }
      putchar('\n');
    }
  }
  freeWordList(words);
  return 0;
}
