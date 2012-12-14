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

static inline int pointValue(char *word, unsigned minWordLength)
{
  if(strlen(word) >= minWordLength) {
    switch(strlen(word)) {
      case 3:
      case 4: return 1;
      case 5: return 2;
      case 6: return 3;
      case 7: return 5;
      default: return 10;
    }
  }
  return 0;
}

static int totalPoints(wordList words, unsigned minWordLength)
{
  assert(words.words);
  int points = 0;
  for(unsigned i=0; i<words.numWords; i++) {
    points += pointValue(words.words[i], minWordLength);
  }
  return points;
}

static void printWordList(wordList words, unsigned minWordLength)
{
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
}

int main(int argc, char *argv[])
{
  if(argc != 2) {
    printf("Usage: %s <letters>\n", argv[0]);
    return 1;
  }
  trieNodeIdentifier dict = makeDictionary(stdin);
  assert(dict);
  char *letters = argv[1];
  removeQUs(letters);
  int numLetters = strlen(letters);
  int boardSize = sqrt(numLetters);
  unsigned minWordLength = boardSize <= 4 ? 3 : 4;
  assert(boardSize * boardSize == numLetters);
  wordList words = solveBoard(dict, letters, boardSize);
  trieNode_freeAll();
  fprintf(stderr, "There are %d words\n", words.numWords);
  fprintf(stderr, "There are %d points\n", totalPoints(words, minWordLength));
  printWordList(words, minWordLength);
  freeWordList(words);
  return 0;
}
