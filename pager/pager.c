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

/*
 * The exercises description is written in English, so I'm delivering the
 * answers in English.
 *
 * The programs will be compiled using libc-2.28 and gcc-8.3.0.
 *
 * All three exercises contain a Makefile to compile the code.
 *
 * I didn't have much diffilculty when solving exercises 1 and 3.
 *
 * Exercise 2, a bit longer, was a bit more difficult. The aproach of printing
 * the lines as they are read has a catch. If the line is two big it must be
 * broken down, so it is possible to reach the columns limit when reading a
 * line. So, the header and the extra new line may be inserted when printing
 * a page or when printing a line, making the program somewhat more complex.
 *
 * Exercise 2:
 * ------------
 *
 * There are two programs in this solution:
 *
 * "checkPager.sh" - Bash script to check the pager.
 * "pager.c" - The pager.
 *
 * The description points to an "extra feature", we assume it is compulsary.
 *
 * The description doesn't mention any kind of user input. It is only enabled
 * when the input file is NOT stdin.
 *
 * We need to accept an arbitrary number of file names from the command line.
 * So we define and array of pointers to strings, a pointer for every file
 * name.
 *
 * We can keep an integer variable to count the number of files. Since we
 * don't know the array length, it must be dynamically allocated.
 * 
 * The above is quite precisely "argc" and "argv" ANSI C definition. So we
 * only need to use them to store and get filenames.
 *
 * We must read every file line by line. We can use C standard libraries
 * functions for that task. We could use "gets" wich is portable but not so
 * safe. We are using GNU C library "getline" instead.
 *
 * Since the exercise compels us to use gcc, we might reasonable assume that
 * GNU C library will also be available, so "getline" will be allowed for us.
 *
 * The exercise asks to concatenate one text file after the other and
 * show it to the screen in 80 line pages. We assume a length of 80 lines of
 * text, as they are read from the file, without counting the one at the
 * begining showing a page number, or the one at the end to allow user input.
 *
 * Here is a brief description for the pager:
 *
 * First we step over every file on the function "pager", sending a stream
 * pointer to "prnPage".
 *
 * In this latter function we step over every line as we print a header for
 * every page. A footer invites the user to continue by hitting "Intro" key.
 * Since the line count may reach 80 when the lines are broken in blocks, the
 * footer and the header may be shown at the function "prnLine". The variable
 * "delayed" allows us to print the header at the begining of a new page,
 * making sure it is not printed when there aren't any more pages to print.
 *
 * Finally, we write code for printing a single line at "prnLine", we split
 * the lines in blocks with "nCols" size and print each block. It uses
 * pointer arithmetics to do so:
 *
 * "* mark" keeps track of next block's first character and
 * "block [nCols + 1]" stores every block.
 *
 * As you can see the aproach is to print the lines on the fly, that is, as
 * they are read.
 *
 * We could also set up a buffer to store at least 80 lines and then print
 * every page at once. That would easy page formatting at the cost of having
 * to develop a memory management system.
 */

/*
 * Improvements:
 *
 * 1.- We are using pointer arithmetics. So we would use a memory debugger
 * (like valgrind) to make sure we don't get buffer overruns or other memory
 * issues.
 * 2.- We could write some more tests.
 * 3.- The name of the file could be printed.
 * 4.- The number of lines and total bytes for every file could be printed.
 * 5.- The Linux terminal could be set to non-canonical mode to get keyboard
 * user input without having to resort to "gethar ()", or use ncurses.
 * 6.- We could allow the user navigate through the listed files with arrow keys
 * and the like.
 *
 * To implement those improvements we could:
 *
 * 1.- Just run the program under valgrind with the makefile.
 * 2.- We must add new text files and run the program against them. Those
 * files may have specific line lengths to check the program behaviour. We can
 * use "gcov" to ensure the desired code coverage.
 * 3.- It can easily be printed at "prnHeader" function.
 * 4.- Same as above.
 * 5.- It may require to load all input data to a buffer and then show it on
 * the terminal with ncurses. The program may need a new logic like the MVC
 * pattern.
 * 6.- Like above.
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

