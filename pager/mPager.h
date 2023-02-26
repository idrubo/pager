
#ifndef mPager_h 
#define mPager_h

#include <unistd.h>
#include <errno.h>

#include "memory.h"

#define N_ROWS_MAX 80ul
#define N_COLS_MAX 40ul

struct pages;

void pager (int argc, char * argv [], int iFiles);
void loadPage (FILE * textF, struct pages * page);
void fmtPage (struct pages * page);
int splitLines (struct pages * page, char * line);
void prnHeader (int nPages);
void userInput (void);

int getLines (void);
int getCols  (void);

int parseCmd (int argc, char * argv []);
void prnUsage (void);
unsigned long xstrtoul (const char * s);

void xfopen (FILE ** f, char * name);
void xfclose (FILE * f);

#endif /* mPager_h */

