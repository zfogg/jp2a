#!/bin/bash


cd `dirname $0`

aclocal
autoconf
autoheader
automake --add-missing
./configure --prefix=$HOME
make
