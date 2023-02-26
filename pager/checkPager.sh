#!/bin/bash

if [ ! -x 'mPager' ] ; then
  echo 'Compiling mPager'
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
echo 'command: ./mPager -l 30 -c 25 file1.txt file2.txt file3.txt'
echo 'Hit intro to run mPager' ; read
./mPager -l 30 -c 25 file1.txt file2.txt file3.txt

# Leaving the default of 80 lines per page
# and 40 of colums per line
# Reading from the example files
clear
echo '------------------'
echo '--- Second run ---'
echo '------------------'
echo
echo 'command: ./mPager file1.txt file2.txt file3.txt'
echo 'Hit intro to run mPager' ; read
./mPager file1.txt file2.txt file3.txt

# Reading from stdin
clear
echo '------------------'
echo '--- Third run ---'
echo '------------------'
echo
echo 'command: cat file1.txt file2.txt file3.txt | ./mPager'
echo 'Hit intro to run mPager' ; read
cat file1.txt file2.txt file3.txt | ./mPager

exit 0

