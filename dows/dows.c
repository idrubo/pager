#include <string.h>

/*
 * Exercise 3:
 * ------------
 *
 * There is two *.c files in this solution:
 *
 * "dows.c" - Is a verbatim copy of the exercise listing.
 * "check.c" - Is a program that checks the corrected version of "get_dow" and
 * the modified version that tries to improve it.
 *
 * The routine "get_dow" searches the string array "* dows []" through an,
 * index "dow". That index is coded as a binary number. It maps the index to
 * the string array the following way:
 *
 *  1 = 0000001 -> "Sun-"
 *  2 = 0000010 -> "Mon-"
 *  4 = 0000100 -> "Tue-"
 *  8 = 0001000 -> "Wed-"
 * 16 = 0010000 -> "Thu-"
 * 32 = 0100000 -> "Fri-"
 * 64 = 1000000 -> "sat-"
 *
 * If we add some of those binary numbers and call "get_dow", we get a string
 * with all the days of the week corresponding to the terms of the sum,
 * thus:
 *
 * 0000010 + 0000100 + 0001000 = 0001110 = 2 + 4 + 8 = 14
 *
 * returns the string: "Mon-Tue-Wed"
 *
 * Issues found:
 *
 * Sintactic errors can easily be checked by compiling the code.
 * 
 * "dow_to_string" incorrectly calls "get_dow". It must be called with TWO
 * parameters instead of only one, as we read on the code.
 * The compiler will issue an error.
 * To solve it we must pass the required parameters.
 *
 * "dow_to_string" returns a pointer to a local variable.
 * That is incorrect because local variables will be destroyed as soon as the
 * function leaves its scope.
 * The compiler will issue a warning.
 * To solve it we can pas "dowt" as a parameter or dynamically allocate it. If
 * we choose to pass "dowt" as parameter it is mostly the same than calling
 * "get_dow" directly, so we can eliminate it.
 *
 * Finally, the sentence "dowt [4] = '\0'" is slightly imprecise. Since in C
 * an array subindex starts counting from 0, it should be "dowt [3] = '\0'"
 */

/*
 * We made the code a bit clearer:
 *
 * By using "strcat" instead of the pointer arithmetic, we can eliminate that
 * pointer arithmetic and along wiht the "pos" variable.
 *
 * By declaring a constant as:
 * char * notFound = " - ";
 * we can simplify a bit the case when "found = 0", by using "strcpy".
 *
 * By initializing "found" as it is declared.
 *
 * And Finally, by eliminating "dow_to_string".
 *
 * The changes can be easily seen at the file "check.c". A Makefile is present
 * so it is simple to check the changes.
 */

char * dows [] = {"Sun-", "Mon-", "Tue-", "Wed-", "Thu-", "Fri-", "Sat-"};

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
    dowt [pos -1] = '\0';
  else
  {
    memcpy (dowt, " - ", 3);
    dowt [4] = '\0';
  }
}

char * dow_to_string (unsigned char dow)
{
  char dowt [30];

  get_dow (dow);

  return dowt;
}

