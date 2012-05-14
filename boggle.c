#include "trie.h"
#include "boggle.h"
#include "getline.h"
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
    char *word = getline(fp);
    if(word) {
      char c;
      for(char *letter = word; (c = *letter) != '\0'; letter++) {
        if(!isalpha(c))
          continue;
      }
      trieNode_add(dict, word);
    }
  }
  return dict;
}

typedef uint64_t usedLetters; /* represents a bit array of letters used in the
                                 current boggle word. 0 means unused. The least
                                 significant bit refers to the first entry in
                                 the letters array */

typedef struct queueEntry {
  char *letters; /* Letters in the word so far */
  int numLetters;
  uint64_t usedLetters; /* represents a bit array of letters used in the current
                           boggle word. 0 means unused. The least significant
                           bit refers to the first entry in the letters array */
  int currentIndex;
  trieNode *currentDict;
} queueEntry;

typedef struct boggleNode {
  queueEntry value;
  struct boggleNode *next;
} boggleNode;

typedef struct boggleQueue {
  boggleNode *head;
} boggleQueue;

static void addQueueEntry(boggleQueue *queue, queueEntry entry) {
  boggleNode *node = malloc(sizeof(*node));
  assert(node);
  node->value = entry;
  node->next = queue->head;
  queue->head = node;
}

static queueEntry popQueueEntry(boggleQueue *queue) {
  assert(queue && queue->head);
  boggleNode *node = queue->head;
  queue->head = node->next;
  queueEntry entry = node->value;
  free(node);
  return entry;
}

static void addToQueueIfValid(boggleQueue *queue, queueEntry prefix,
                              int row, int col, int boardSize,
                              char *board)
{
  uint64_t one = 1;
  int index = row * boardSize + col;
  bool isValid = (row >= 0) && (col >= 0) &&
                 (row < boardSize) && (col < boardSize) &&
                 !(prefix.usedLetters & (one << index));
  assert(board);
  if(isValid) {
    queueEntry entry = prefix;
    entry.numLetters++;
    char c = board[index];
    entry.letters = malloc((entry.numLetters + 1) * sizeof(*(entry.letters)));
    if(prefix.letters)
      strcpy(entry.letters, prefix.letters);
    entry.letters[entry.numLetters - 1] = c;
    entry.letters[entry.numLetters] = '\0';
    entry.currentIndex = index;
    entry.currentDict = trieNode_at(prefix.currentDict, c);
    if(!entry.currentDict) {
      return;
    }
    uint64_t one = 1;
    entry.usedLetters |= one << index;

    addQueueEntry(queue, entry);
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

  boggleQueue queue;
  queue.head = NULL;
  /* Add each letter to the queue */
  for(unsigned row = 0; row<boardSize; row++) {
    for(unsigned col = 0; col<boardSize; col++) {
      queueEntry entry;
      entry.numLetters = 0;
      entry.usedLetters = 0;
      entry.letters = NULL;
      entry.currentDict = dict;
      addToQueueIfValid(&queue, entry, row, col, boardSize, board);
    }
  }

  /* Process the queue */
  const int directions[][2] = {{-1, -1}, {-1, 0}, {-1, 1},
                               { 0, -1},          { 0, 1},
                               { 1, -1}, { 1, 0}, { 1, 1}};
  const int numDirections = sizeof(directions)/sizeof(*directions);
  while(queue.head) {
    queueEntry entry = popQueueEntry(&queue);
    if(trieNode_isWord(entry.currentDict)) {
      addWord(&words, entry.letters);
    }

    for(int i=0; i<numDirections; i++) {
      int row = entry.currentIndex / boardSize + directions[i][0];
      int col = entry.currentIndex % boardSize + directions[i][1];
      addToQueueIfValid(&queue, entry, row, col, boardSize, board);
    }
  }
  return words;
}
