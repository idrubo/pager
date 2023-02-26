#include <string.h>
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

