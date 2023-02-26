/* Memory management */

#ifndef memory_h 
#define memory_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mPager.h"

#define B_SIZE 1024ul

struct pages
{
  char * buffer;
  unsigned long rd, wr;
  unsigned long nBlocks, nLines, nBytes;
};

void initPage  (struct pages * page);
void freePage  (struct pages * page);
void resetPage (struct pages * page);

int saveLine (struct pages * page, char * line);
char * loadLine (struct pages * page);

int notEmpty (struct pages * page);

void xmalloc (char ** v, size_t s);
char * xrealloc (char * b, size_t s);

#endif /* memory_h */

