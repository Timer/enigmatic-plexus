#!/bin/bash
cd $(dirname "$0")
CC=gcc
OS=`uname`
if command -v clang-format >/dev/null 2>&1; then
  echo "Linting..."
  clang-format -i *.c *.h
fi
echo "Compiling..."
if [ $OS == "Darwin" ]; then
  CC=gcc-5
fi
echo "... using $CC."
rm *.out
$CC -std=gnu99 -fopenmp -lmpi -c $(find . -name \*.c) -lm
echo "Building..."
$CC -std=gnu99 -fopenmp -lmpi $(find . -name \*.o -not -name k2.o) -o test.out -lm
$CC -std=gnu99 -fopenmp -lmpi $(find . -name \*.o -not -name test.o) -o k2.out -lm
rm $(find . -name \*.o)
