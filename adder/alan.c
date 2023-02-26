#include <stdio.h>
#include <assert.h>

#define LENGTH 5
#define START  1
#define END    5
#define LAST   4

int dummy_adder (int * a5);
void sug_adder (int * s5);

int main (void)
{
  int i;
  int s, a5 [LENGTH]; // a5 is declared with a memory size.

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
  
  for (i = START; i < END; i++) // We must check the limits for the for iteration.
    s5 [i] = s5 [i - 1] + (i + 1);

  assert (s5 [0] == 1);
  assert (START == 1);
  assert (END == LENGTH);
  assert (LAST == LENGTH - 1);
  printf ("Assertions ok\n");

  return s5 [LAST]; // The subindex for s5 [4] must be checked.
}

void sug_adder (int * s5)
{
  int i, s = 0;

  for (i = 1; i <= 5; i++) s += i;
  for (i = 0; i < 5; i++) s5 [i] = s;
}

