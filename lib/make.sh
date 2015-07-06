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
rm *.out
$CC -lm -c $(find . -name \*.c)
echo "Building..."
$CC $(find . -name \*.o -not -name k2.o) -o test.out
$CC $(find . -name \*.o -not -name test.o) -o k2.out
rm $(find . -name \*.o)
