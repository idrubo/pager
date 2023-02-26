#include "memory.h"

void initPage (struct pages *page)
{
  xmalloc (& (page->buffer), B_SIZE);

  page->rd = 0lu;
  page->wr = 0lu;

  page->nLines  = 0lu;
  page->nBytes  = 0lu;
  page->nBlocks = 1lu;
}

void freePage (struct pages * page)
{
  free (page->buffer);
}

void resetPage (struct pages * page)
{
  page->rd = 0lu;
  page->wr = 0lu;
}

int saveLine (struct pages * page, char * line)
{
  unsigned long toSave = strlen (line) + 1lu;

  if ((page->nBytes + toSave) > (page->nBlocks * B_SIZE))
  {
    page->nBlocks++;
    page->buffer = xrealloc (page->buffer, page->nBlocks * B_SIZE);
  }

  page->nLines++;
  page->nBytes += toSave;

  strcpy (page->buffer + page->wr, line);

  page->wr += toSave;

  if (page->nLines >= getLines ()) return 1;

  return 0;
}

char * loadLine (struct pages * page)
{
  char * n = page->buffer + page->rd;
  unsigned long toLoad = strlen (n) + 1lu;

  page->nLines--;
  page->nBytes -= toLoad;
  page->rd     += toLoad;

  return n;
}

int notEmpty (struct pages * page) { return page->nLines; }

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

