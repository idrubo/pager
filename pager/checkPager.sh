#!/bin/bash

if [ ! -x 'pager' ] ; then
  echo 'Compiling the pager'
  make
fi

# Changing the number of lines per page
# and the number of colums per line
# Reading from the example files
echo
echo '-----------------'
echo '--- First run ---'
echo '-----------------'
echo
./pager -l 30 -c 25 file1.txt file2.txt file3.txt

# Leaving the default of 80 lines per page
# and 40 of colums per line
# Reading from the example files
echo
echo '------------------'
echo '--- Second run ---'
echo '------------------'
echo
./pager file1.txt file2.txt file3.txt

# Reading from stdin
echo
echo '------------------'
echo '--- Third run ---'
echo '------------------'
echo
cat file1.txt file2.txt file3.txt | ./pager

exit 0

