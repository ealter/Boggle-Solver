#define START_SIZE 128 /* initial buffer size */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "getline.h"

static char *buf = NULL; // holds line being read
static int buf_size = START_SIZE; // size of buf if non-NULL
static inline void *checkptr(void *p) { // barf if p is NULL
  if (p == NULL) {
    fprintf(stderr, "%s, line %d: out of memory\n",
        __FILE__, __LINE__);
    exit(2);
  }
  return p;
}

static void double_buf(void) {
  buf_size *= 2;
  buf = checkptr(realloc(buf, buf_size));
}

char *getline (FILE *fp) {
  if (buf==NULL)
    buf = checkptr(malloc(buf_size));
  char *p = fgets(buf, buf_size, fp);
  if (p == NULL) { // end of file; avoid leaks
    free(buf);
    buf = NULL;
    return NULL;
  }
  while (buf[strlen(buf)-1] != '\n') { // failed to get whole line
    double_buf();
    if (fgets(buf+strlen(buf),buf_size-strlen(buf),fp)==NULL) { // EOF
      return buf; // return partial line
    }
  }
  return buf;
}
