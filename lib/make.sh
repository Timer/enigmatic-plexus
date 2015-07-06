#!/bin/sh
cd $(dirname "$0")
CC=gcc
OS=`uname`
echo "Compiling..."
if [ $OS == "Darwin" ]; then
  CC=gcc-5
fi
$CC -c $(find . -name \*.c)
echo "Building..."
$CC $(find . -name \*.o -not -name k2.o) -o test.out
$CC $(find . -name \*.o -not -name test.o) -o k2.out
rm $(find . -name \*.o)
