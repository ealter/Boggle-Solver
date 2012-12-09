#ifndef GETLINE_BOGGLE_INCLUDED
#define GETLINE_BOGGLE_INCLUDED

#include <stdio.h>

//Reads a line from the file. The return value may be overwritten on the next
//call to getLineBoggle. This returns NULL if no characters are read (EOF).
extern char *getlineBoggle(FILE *fp);

#endif

