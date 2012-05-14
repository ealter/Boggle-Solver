SOURCES  = getlineBoggle.c trie.c boggle.c main.c
OBJECTS  = $(SOURCES:.c=.o)
RESULT   = boggle

CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -std=c99 -O2
RM = rm -f 

.SUFFIXES:
.SUFFIXES: .c .o

$(RESULT): $(OBJECTS)
	$(CC) -o $@ $(OBJECTS) -lm

.c.o:
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $<

clean:
	$(RM) $(RESULT) *.o *.core core *~

getlineBoggle.o: getlineBoggle.h getlineBoggle.c
trie.o: trie.h trie.c
boggle.o: trie.h boggle.h getlineBoggle.h boggle.c
main.o: boggle.h trie.h main.c

