#include "memory.h"

/* Initialize a memory page */
void initPage (struct pages *page)
{
  xmalloc (& (page->buffer), B_SIZE);

  page->rd = 0lu; /* Keeps track of the next line to read */
  page->wr = 0lu; /* Keeps track of the next line to write */

  page->nLines  = 0lu; /* Number of lines stored */
  page->nBytes  = 0lu; /* Total bytes stored */
  page->nBlocks = 1lu; /* Total page memory size is: nBlocks * B_SIZE */
}

/* Releasing allocated memory */
void freePage (struct pages * page)
{
  free (page->buffer);
}

/* When the queue gets empty (all lines are printed) the page is reset */
void resetPage (struct pages * page)
{
  page->rd = 0lu;
  page->wr = 0lu;
}

/* Save a line */
int saveLine (struct pages * page, char * line)
{
  unsigned long toSave = strlen (line) + 1lu;

  /* If we get out of space we ask for more memory */
  if ((page->nBytes + toSave) > (page->nBlocks * B_SIZE))
  {
    page->nBlocks++;
    page->buffer = xrealloc (page->buffer, page->nBlocks * B_SIZE);
  }

  /* One more line saved */
  page->nLines++;
  page->nBytes += toSave;

  strcpy (page->buffer + page->wr, line);

  /* Pointing to the next line to write */
  page->wr += toSave;

  /* If there are "nCols" lines, they can be printed */
  if (page->nLines >= getLines ()) return 1;

  return 0;
}

/* Eliminate a line once is printed. */
char * loadLine (struct pages * page)
{
  char * n = page->buffer + page->rd;
  unsigned long toLoad = strlen (n) + 1lu;

  page->nLines--;
  page->nBytes -= toLoad;

  /* Pointing to the next line to read */
  page->rd     += toLoad;

  return n;
}

/*
 * Returns true if there are still lines to read, false if the queue is empty
 */
int notEmpty (struct pages * page) { return page->nLines; }

/* Checking for errors when allocating memory */
void xmalloc (char ** b, size_t s)
{
  * b = malloc (s);
  if (* b == NULL)
  {
    printf ("Not enough memory.\n");
    exit (EXIT_FAILURE);
  }
}

char * xrealloc (char * b, size_t s)
{
  char * nb;

  nb = realloc (b, s);
  if (nb == NULL)
  {
    printf ("Not enough memory.\n");
    exit (EXIT_FAILURE);
   }

  return nb;
}

