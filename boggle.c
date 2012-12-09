#include "trie.h"
#include "boggle.h"
#include "getlineBoggle.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <ctype.h>

static void addWord(wordList *words, char *word)
{
  assert(words && words->words);
  words->numWords++;
  while(words->numWords > words->arraySize) {
    words->arraySize *= 2;
    words->words = realloc(words->words,
                           words->arraySize * sizeof(*(words->words)));
    for(unsigned i=words->numWords; i<words->arraySize; i++)
      words->words[i] = NULL;
  }
  words->words[words->numWords - 1] = word;
}

trieNode *makeDictionary(FILE *fp)
{
  trieNode *dict = trieNode_new();

  while(!feof(fp)) {
    char *word = getlineBoggle(fp);
    if(word) {
      trieNode_add(dict, word);
    }
  }
  return dict;
}

typedef uint64_t usedLetters; /* represents a bit array of letters used in the
                                 current boggle word. 0 means unused. The least
                                 significant bit refers to the first entry in
                                 the letters array */

static inline bool isValidMove(unsigned index, uint64_t usedLetters)
{
  uint64_t one = 1;
  return !(usedLetters & (one << index));
}

static void _solveBoard(trieNode *currentDict, const char *board, unsigned boardSize,
                        uint64_t usedLetters, int currentIndex, wordList *words)
{
  if(!currentDict)
    return;
  assert(board && words);

  const uint64_t one = 1;
  unsigned currentRow = currentIndex / boardSize;
  unsigned currentCol = currentIndex % boardSize;
  unsigned index;
  trieNode *dict;
#define MOVE(deltaRow, deltaCol)                                   \
      index = currentIndex + deltaCol + deltaRow * boardSize;      \
      if(isValidMove(index, usedLetters)) {                        \
        dict = trieNode_at(currentDict, board[index]);             \
        if(dict) {                                                 \
          _solveBoard(dict, board, boardSize,                      \
                      usedLetters | (one << index), index, words); \
        }                                                          \
      }
  if(currentRow != 0) {
    if(currentCol != 0) {
      MOVE(-1,-1);
    }
    MOVE(-1, 0);
    if(currentCol + 1 < boardSize) {
      MOVE(-1, 1);
    }
  }
  if(currentCol > 0) {
    MOVE( 0,-1);
  }
  if(currentCol + 1 < boardSize) {
    MOVE( 0, 1);
  }
  if(currentRow + 1 < boardSize) {
    if(currentCol != 0) {
      MOVE( 1,-1);
    }
    MOVE( 1, 0);
    if(currentCol + 1 < boardSize) {
      MOVE( 1, 1);
    }
  }

  if(trieNode_isWord(currentDict)) {
    addWord(words, trieNode_toString(currentDict));
  }
}

wordList solveBoard(trieNode *dict, char *board, unsigned boardSize)
{
  assert(dict && board);
  assert(strlen(board) == boardSize * boardSize);
  assert(boardSize >= 1 && boardSize <= 8);

  wordList words;
  words.arraySize = 100;
  words.numWords = 0;
  words.words = calloc(1, words.arraySize * sizeof(*(words.words)));

  uint64_t one = 1;
  for(unsigned row = 0; row<boardSize; row++) {
    for(unsigned col = 0; col<boardSize; col++) {
      int index = row * boardSize + col;
      _solveBoard(trieNode_at(dict, board[index]), board, boardSize,
                  one << index, index, &words);
    }
  }
  return words;
}

void removeQUs(char *board) {
  int i=0, shift=0;
  for(char c; (c = board[i]) != '\0'; i++) {
    board[i - shift] = c;
    if(c == 'q' && board[i + 1] == 'u') {
      shift++;
      i++;
    }
  }
  board[i - shift] = '\0';
}

