/* To format and print a page */

#ifndef format_h 
#define format_h

#include <stdio.h>

#include "mPager.h"
#include "memory.h"

struct pages;

void fmtPage (struct pages * page);
void prnHeader (int nPages);
void userInput (void);

#endif /* format_h */

