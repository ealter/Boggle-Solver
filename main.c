#include "boggle.h"
#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[])
{
  if(argc != 2) {
    printf("Usage: %s <letters>\n", argv[0]);
    return 1;
  }
  trieNode *dict = makeDictionary(stdin);
  puts("Got dictionary\n");
  assert(dict);
  char *letters = argv[1];
  int numLetters = strlen(letters);
  int boardSize = sqrt(numLetters);
  unsigned minWordLength = boardSize <= 4 ? 3 : 4;
  assert(boardSize * boardSize == numLetters);
  wordList words = solveBoard(dict, letters, boardSize);
  trieNode_free(dict);
  printf("There are %d words\n", words.numWords);
  assert(words.words);
  for(unsigned i=0; i<words.numWords; i++) {
    char *word = words.words[i];
    if(strlen(word) >= minWordLength)
      puts(word);
  }
  for(unsigned i=0; i<words.numWords; i++)
    free(words.words[i]);
  free(words.words);
  return 0;
}
