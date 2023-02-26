#include "mPager.h"
#include "memory.h"

/*
* Memory manangement:
* 
* Put all memory management files at "memory.c"
* 
* 1.- Allocate a page with malloc.
* 
* 2.- Start reading lines and fill the page.
* 2.1.- Keep track of the number of lines.
* 2.2.- Keep track of the amount of memory used.
* 2.3.- Realloc memory as needed.
* 
* 3.- Format and print the page.
* 3.1.- Print the header.
* 3.2.- Print 80 lines.
* 3.3.- Print the footer and ask for user input.
* 3.4.- Keep track of the number of lines.
* 3.5.- Keep track of the amount of memory used.
* 3.6.- Repeat 3.1 to 3.5 until no lines are left.
* 
* 4.- Repeat 2 and 3 until the end of the file.
*/

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

  while (! feof (textF))
  {
    if (getline (& line, & N, textF) == EOF) break;

    if (strlen (line) > nCols)
      full = splitLines (page, line);
    else
      full = saveLine (page, line);

    if (full) fmtPage (page);
  }

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

void fmtPage (struct pages * page)
{
  static int nPages = 1, lv = 0, h = 1;

 /* 
  * El bucle debe contemplar que el buffer puede contener más de una página.
  * Además las páginas no tendrán una longitud fija.
  * 1.- Bolcar el buffer completo en la pantalla.
  * 2.- Mantener un registro del número de lineas_volcadas.
  * 3.- Volcar un número de lineas = nRows - lineas_volcadas.
  * 4.- lineas_volcadas = nRows.
  * 5.- Repetir 3 y 4 hasta que el buffer esté vacío.
  */

  while (notEmpty (page))
  {
    if (h)
    {
      h = 0;
      prnHeader (nPages++);
    }

    lv++;

    printf ("%s", loadLine (page));

    if (lv == nRows)
    {
      h = 1, lv = 0;
      printf ("\n");
      if (isatty (STDIN_FILENO)) userInput ();
    }
  }
  resetPage (page);
}

/* Printing a header with the page number */
void prnHeader (int nPages)
{
  int j;
  for (j = 0; j < (nCols / 2); j++) printf (" ");
  printf ("--- Page: %d ---\n", nPages);
}

/* Asking for user input */
void userInput (void)
{
  printf ("Hit intro to continue.");
  while (getchar () != '\n');
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

