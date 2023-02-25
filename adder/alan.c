#include <stdio.h>
#include <assert.h>

#define LENGTH 5
#define START  1
#define END    5
#define LAST   4

/*
 *
 * Exercise 1:
 * ------------
 *
 * There are two programs in this solution:
 *
 * "alan.c" - Is the program than add asserts to Alan's program and makes it
 * run.
 *
 * "adder.c" - Exams the formula to compute the sum of the first n natural
 * numbers.
 *
 * Issues found:
 *
 * Most of the issues when using C arrays are memory related. If we try to
 * write passed an array limit, we will most likely get a "segmentation fault"
 * from the operating system and our program will be killed. Buffer overruns
 * is another common pitfall when using arrays. We can just look for thoses
 * issues.
 *
 * In first place, the length of the array must match the one assumed by
 * "dummy_adder". So, we assert the length of the array when it is declared.
 *
 * The upper limit on the "for" iteration must match the length of the array,
 * too. The lower limit of that iteration must be checked, we shall get a
 * wrong result if we do not.
 *
 * For the iteration to work "s5 [0]" must be properly initialized.
 *
 * Finally the subindex for s5 [4] must be checked to return the correct
 * amount.
 *
 * If we get the result with and array of 5 elements with the sum in every
 * position we can skip the following asserts:
 *
 * "assert (LAST == 4);"
 * "assert (s5 [0] == 1);"
 *
 */

int dummy_adder (int * a5);
void sug_adder (int * s5);

int main (void)
{
  int i;
  int s, a5 [LENGTH]; // a5 must have the proper size.

  assert (LENGTH == 5);

  printf ("Alan solution:\n");
  s = dummy_adder (a5);

  printf ("sum = %d.\n", s);

  for (i = 0; i < 5; i++)
    printf ("a5 [%d] = %d.\n", i, a5 [i]);

  printf ("\nSuggested solution:\n");
  sug_adder (a5);

  for (i = 0; i < 5; i++)
    printf ("a5 [%d] = %d.\n", i, a5 [i]);

  return 0;
}

int dummy_adder (int * s5)
{
  int i;

  s5 [0] = 1; // s [0] must be initialized
  assert (s5 [0] == 1);

  assert (START == 1);
  assert (END == 5);
  
  for (i = START; i < END; i++) // We must check the limits for the for iteration.
    s5 [i] = s5 [i - 1] + (i + 1);

  assert (LAST == 4);

  printf ("Assertions ok\n");

  return s5 [LAST]; // The subindex for s5 [4] must be checked.
}

void sug_adder (int * s5)
{
  int i, s = 0;

  for (i = 1; i <= 5; i++) s += i;
  for (i = 0; i < 5; i++) s5 [i] = s;
}

