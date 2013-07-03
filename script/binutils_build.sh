#!/bin/sh
ROOT=$(pwd)
cd ./extern/binutils
git checkout binutils-2_23_2
cd $ROOT
cd build/binutils
if [ ! -e Makefile ]; then
  echo "configuring binutils"
  ../../extern/binutils/configure --target=arm-none-eabi --enable-interwork --prefix=$ROOT/toolchain
fi
echo "building binutils"
make
echo "installing binutils"
make install
cd $ROOT