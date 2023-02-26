#include <stdio.h>
#include <string.h>

void prnDow (unsigned char dow, char * dowt);
void prnDowS (unsigned char dow, char * dowt);
static void get_dow (unsigned char dow, char * dowt);
static void get_dowS (unsigned char dow, char * dowt);

char * dows [] = {"Sun-", "Mon-", "Tue-", "Wed-", "Thu-", "Fri-", "Sat-"};
char * notFound = " - ";

#define SUN 1
#define MON 2
#define TUE 4
#define WED 8
#define THU 16
#define FRI 32
#define SAT 64

int main (void)
{
  unsigned char dow = 0;
  char dowt [30];

  printf ("Original function:\n");

  prnDow (  0, dowt);
  prnDow (128, dowt);
  prnDow (  1, dowt);
  prnDow (  4, dowt);
  prnDow ( 14, dowt);
  prnDow ( 64, dowt);
  prnDow (127, dowt);

  printf ("\nSimplified version:\n");

  prnDowS (  0, dowt);
  prnDowS (128, dowt);
  prnDowS (  1, dowt);
  prnDowS (  4, dowt);
  prnDowS ( 14, dowt);
  prnDowS ( 64, dowt);
  prnDowS (127, dowt);

  printf ("\nUsing symbolic constants:\n");

  prnDowS (  0, dowt);
  prnDowS (128, dowt);
  prnDowS (SUN, dowt);
  prnDowS (TUE, dowt);

  dow = MON + TUE + WED;
  prnDowS (dow, dowt);

  prnDowS (SAT, dowt);

  dow = SUN + MON + TUE + WED + THU + FRI + SAT;
  prnDowS (dow, dowt);

  return 0;
}

/* Original function */
static void get_dow (unsigned char dow, char * dowt)
{
  int i, found, pos;

  found = 0;
  dowt [0] = '\0';
  pos = 0;

  for (i = 0; i < 7; i++)
  {
    if (dow & ( 1 << i ))
    {
      memcpy (dowt + pos, dows [i], strlen (dows [i]));
      pos += strlen (dows [i]);
      found = 1;
    }
  }

  if (found)
    dowt [pos - 1] = '\0';
  else
  {
    memcpy (dowt, " - ", 3lu);
    dowt [3] = '\0';
  }
}

/* Simplified version */
static void get_dowS (unsigned char dow, char * dowt)
{
  int i, found = 0; /* "pos" is eliminated */

  dowt [0] = '\0';

  for (i = 0; i < 7; i++)
  {
    if (dow & ( 1 << i ))
    {
      /* "strcat" is used instead of pointer arithmetic */
      strcat (dowt, dows [i]);
      found = 1;
    }
  }

  if (found)
    dowt [strlen (dowt) - 1] = '\0';
  else
    strcpy (dowt, notFound); /* We use a predefined strig constant */
}

void prnDow (unsigned char dow, char * dowt)
{
  get_dow (dow, dowt);
  printf ("dow = %3d, dowt: %s\n", dow, dowt);
}

void prnDowS (unsigned char dow, char * dowt)
{
  get_dowS (dow, dowt);
  printf ("dow = %3d, dowt: %s\n", dow, dowt);
}

