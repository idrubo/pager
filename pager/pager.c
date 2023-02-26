#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define N_ROWS_MAX 80ul
#define N_COLS_MAX 40ul

void pager (int argc, char * argv [], int iFiles);
void prnPage (FILE * textF);
int prnLine (char * line, int * nLines, int * nPages);
int checkPage (int * nLines, int * nPages);
void prnHeader (int nPages);
void userInput (void);

int parseCmd (int argc, char * argv []);
void prnUsage (void);
unsigned long xstrtoul (const char * s);

void xfopen (FILE ** f, char * name);
void xfclose (FILE * f);

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

  /* If there are valid file names at the command line, we process them */
  if (iFiles < argc)
  {
    prnHeader (1);

    for (i = iFiles; i < argc; i++)
    {
      xfopen (& textF, argv [i]);

      while (! feof (textF)) prnPage (textF);

      xfclose (textF);
    }
  }
  /* There are no file names at the command line, we process stdin */
  else
  {
    while (! feof (stdin)) prnPage (stdin);
  }
}

void prnPage (FILE * textF)
{
  int printed = 0;
  static int delayed = 0;
  static int nLines = 0, nPages = 2;
  char * line = NULL;
  size_t N = 0l;

  while (nLines < nRows)
  {
    /* Get a single line */
    if (getline (& line, & N, textF) != EOF)
    {
      /* Print a header with the page number */
      if (delayed)
      {
        delayed = 0;
        prnHeader (nPages++);
      }

      /* Print the line we just read */
      printed = prnLine (line, & nLines, & nPages);

      /*
       * If we reach the page limit, print te extra new line
       * "delayed" will make sure we print the header on the next iteration.
       */
      if ((nLines == nRows) && (! printed))
      {
        nLines = 0, delayed = 1;
        printf ("\n");
        /* Ask for the user input, only if the input is NOT stdin. */
        if (isatty (STDIN_FILENO)) userInput ();
      }
    }
    else break;
  }
}

int prnLine (char * line, int * nLines, int * nPages)
{
  int printed = 0;
  char * mark = NULL, block [nCols + 1];

  mark = line;

  while (strlen (mark))
  {
    /* Break the line in "nCols" blocks */
    if (strlen (mark) > nCols)
    {
      memcpy (block, mark, nCols);
      block [nCols] = '\0';
      mark += nCols;

      printf ("%d:", * nLines + 1);
      printf ("%s\n", block);

      (* nLines)++;
      /* Check if "nLines", the lines already printed, equals "nCols" */
      if (* mark)
        printed = checkPage (nLines, nPages);
    }
    /* The line is now shorter than "nCols" */
    else
    {
      memcpy (block, mark, strlen (mark));
      block [strlen (mark)] = '\0';
      mark += strlen (mark);

      printf ("%d:", * nLines + 1);
      printf ("%s", block);

      (* nLines)++;
      if (* mark)
        printed = checkPage (nLines, nPages);
    }
  }

  return printed;
}

/* 
 * Check if "nLines", the lines already printed, equals "nCols".
 * If it is print the extra new line.
 * If the input is a file, ask for user input.
 */
int checkPage (int * nLines, int * nPages)
{
  int printed = 0;

  if (* nLines == nRows)
  {
    * nLines = 0, printed = 1;
    printf ("\n");
    if (isatty (STDIN_FILENO)) userInput ();
    prnHeader ((* nPages)++);
  }

  return printed;
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

