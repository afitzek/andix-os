#!/bin/bash
. script/libbuild.sh
ROOT=$(pwd)
WHAT=gcc
CFGFLAGS="--target=arm-none-eabi --enable-interwork --prefix=$ROOT/toolchain --with-newlib --with-headers=$ROOT/extern/newlib-2.1.0/newlib/libc/include --enable-languages=c,c++"

cd_mk

check 


if [ ! -e Makefile ]; then
echo "configuring $WHAT"
$ROOT/extern/$WHAT/configure $CFGFLAGS
fi &&


make -j$J all-gcc all-target &&
make install-gcc install-target