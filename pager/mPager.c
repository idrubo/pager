#include "mPager.h"

int nRows = N_ROWS_MAX, nCols = N_COLS_MAX;

int main (int argc, char * argv [])
{
  int iFiles;

  /* Parsing the command line. */
  iFiles = parseCmd (argc, argv);

  /* Actual paging. */
  pager (argc, argv, iFiles);

  return 0;
}

void pager (int argc, char * argv [], int iFiles)
{
  int i;
  FILE * textF = NULL;
  struct pages page;

  initPage (& page);

  /* If there are valid file names at the command line, we process them */
  if (iFiles < argc)
  {
    for (i = iFiles; i < argc; i++)
    {
      xfopen (& textF, argv [i]);

      while (! feof (textF)) loadPage (textF, & page);

      xfclose (textF);
    }
  }
  /* There are no file names at the command line, we process stdin */
  else
  {
    while (! feof (stdin)) loadPage (stdin, & page);
  }

  freePage (& page);
}

void loadPage (FILE * textF, struct pages * page)
{
  int full = 0;
  char * line = NULL;
  size_t N = 0l;

  /* Iterating until the whole file is read */
  while (! feof (textF))
  {
    /* Read line by line */
    if (getline (& line, & N, textF) == EOF) break;

    /* If the line is too big, split it into smaller blocks */
    if (strlen (line) > nCols)
      full = splitLines (page, line);
    /* Or else save it */
    else
      full = saveLine (page, line);

    /* When the memory page is full format it*/
    if (full) fmtPage (page);
  }

  /* There may be still lines in memory */
  fmtPage (page);
}

int splitLines (struct pages * page, char * line)
{
  int full = 0;
  char * mark = NULL, block [nCols + 1];

  mark = line;

  while (strlen (mark))
  {
    /* Break the line in "nCols" blocks */
    if (strlen (mark) > (nCols - 1lu))
    {
      memcpy (block, mark, (nCols - 1lu));
      block [nCols - 1lu] = '\n';
      block [nCols] = '\0';
      mark += nCols;

      full = saveLine (page, block);
    }
    /* The line is now shorter than "nCols" */
    else
    {
      memcpy (block, mark, strlen (mark));
      block [strlen (mark)] = '\0';
      mark += strlen (mark);

      full = saveLine (page, block);
    }
  }

  return full;
}

int getLines (void) { return nRows; }
int getCols  (void) { return nCols; }

/* 
 * Parsing the command line to get the number of lines and columns.
 * "nRows" is forced to be between 1 and 80.
 * "nCols" is forced to be between 1 and 40.
 */
int parseCmd (int argc, char * argv [])
{
  unsigned long l, c;
  char * lvalue = NULL, * cvalue = NULL;
  int ch;

  /* DEBUG */
  /* DEBUG */
  /* DEBUG */
  if (argc == 1)
    if (isatty (STDIN_FILENO))
    {
      printf ("I need a pipe to feed from.\n");
      printf ("Take a look at \"checkPager.sh\".\n");
      exit (EXIT_FAILURE);
    }
  /* DEBUG */
  /* DEBUG */
  /* DEBUG */

  opterr = 0;

  while ((ch = getopt (argc, argv, "l:c:")) != -1)
    switch (ch)
    {
      case 'l':
        lvalue = optarg;
        break;
      case 'c':
        cvalue = optarg;
        break;
      default:
        prnUsage ();
        exit (EXIT_FAILURE);
    }

  if (lvalue)
  {
    l = xstrtoul (lvalue);

    if (l > 80) l = 80;
    else if (l < 1) l = 1;

    nRows = (int) l;
  }

  if (cvalue)
  {
    c = xstrtoul (cvalue);

    if (c > 40) c = 40;
    else if (c < 1) c = 1;

    nCols = (int) c;
  }

  return optind;
}

/* When there are errors at the command line */
void prnUsage (void)
{
  fprintf (stderr, "Usage:\n");
  fprintf (stderr, "IGTtest [-l num.] [-c num.] [file ...]\n\n");
}

/* Convert an ASCII string to a number */
unsigned long xstrtoul (const char * s)
{
  unsigned long parsed;
  char * tail;

  errno = 0;

  parsed = strtoul (s, & tail, 0);

  if ((tail [0] != '\0') || (s [0] == '-'))
  {
    fprintf (stderr, "The argument is not a positive integer.\n");
    prnUsage ();
    exit (EXIT_FAILURE);
  }

  if (errno == ERANGE)
  {
    fprintf (stderr, "Argument out of range.\n");
    prnUsage ();
    exit (EXIT_FAILURE);
  }

  return parsed;
}
/* Checking errors when opening files */
void xfopen (FILE ** f, char * name)
{
  *f = fopen (name, "r");  /* We only need to read files. */
  if (*f == NULL)
  {
    fprintf (stderr, "I can NOT open the file.\n");
    exit (EXIT_FAILURE);
  }
}

/* Checking errors when closing files */
void xfclose (FILE * f)
{
  int rVal;
  rVal = fclose (f);
  if (rVal == EOF)
  {
    fprintf (stderr, "I can NOT close the file.\n");
    exit (EXIT_FAILURE);
  }
}

