#include <stdio.h>
#include <limits.h>
#include <math.h>

#define NMAX 46340

/*
 * The sum of the first n natural numbers is:
 *
 * sn = 1 + 2 + ... + n = (1 + n) * n / 2 
 *
 * s5 = 1 + 2 + 3 + 4 + 5 = 15
 *
 * or:
 *
 * s5 = (1 + 5) * 5 / 2 = 6 * 5 / 2 = 15
 *
 * Issues:
 *
 * We find two issues with the formula.
 *
 * The first one arises because n / 2 is not an integer if n is odd. It can be
 * solved by dividing by 2 at the end, like:
 *
 * sn = ((1 + n) * n) / 2.
 *
 * Note the use of parentheses.
 *
 * The second one is a possible overflow. We should check for an overflow when
 * computing ((1 + n) * n), so:
 *
 * ((1 + n) * n) <= INT_MAX
 *
 *  2
 * n  +  n - INT_MAX <= 0
 *
 * and we get:
 *
 * n = (-1 + sqrt (1 + 4 * INT_MAX)) / 2 
 *
 * that we can compute using floats.
 *
 * The formula can be used to take care of overflows.
 *
 */

int sn (int n);

int main (void)
{
  int n = 5, s;

  double nMaxf = (-1.0 + sqrt (1.0 + 4.0 * (double) INT_MAX)) / 2.0;
  int nMax = (int) floor (nMaxf);

  printf ("INT_MAX = %d\n", INT_MAX);
  printf ("nMaxf = %g\n", nMaxf);
  printf ("nMax = %d\n\n", nMax);

  /* The number given is correct because: */
  printf ("(1 + nMax) * nMax = %d, ", (1 + nMax) * nMax);
  printf ("wich is SMALLER than %d.\n", INT_MAX);

  printf ("(1 + (nMax + 1)) * (nMax + 1) = %.0f, ",
      ((1.0 + ((double) nMax + 1.0)) * ((double) nMax + 1.0)));
  printf ("wich is BIGGER than %d.\n\n", INT_MAX);

  if ((s = sn (n)) > 0)
    printf ("The sum of the first %d natural numbers is %d.\n", n, s);
  else
    printf ("OVERFLOW !!!\n");

  if ((s = sn (NMAX + 1)) > 0)
    printf ("The sum of the first %d natural numbers is %d.\n", n, s);
  else
    printf ("OVERFLOW !!!\n");

  return 0;
}

int sn (int n)
{
  if (n < NMAX) return ((1 + n) * n) / 2;
  else return -1;
}

