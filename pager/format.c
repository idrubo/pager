#include "format.h"

/* Format and print the page */
void fmtPage (struct pages * page)
{
  static int nPages = 1, dl = 0, h = 1;

  /* Iterate until the memory page is empty */
  while (notEmpty (page))
  {
    /* Print the screen page header */
    if (h)
    {
      h = 0;
      prnHeader (nPages++);
    }

    /* Keep track of the number of lines already sent */
    dl++;

    printf ("%s", loadLine (page));

    if (dl == getLines ())
    {
      h = 1, dl = 0;
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
  for (j = 0; j < (getCols () / 2); j++) printf (" ");
  printf ("--- Page: %d ---\n", nPages);
}

/* Asking for user input */
void userInput (void)
{
  printf ("Hit intro to continue.");
  while (getchar () != '\n');
}

