#!/bin/sh
ROOT=$(pwd)
cd ./extern/llvm
git checkout release_33
cd ./tools/clang
git checkout release_33
cd ../../projects/compiler-rt
git checkout release_33
cd $ROOT
cd build/llvm
if [ ! -e Makefile ]; then
  echo "configuring LLVM+CLANG"
  ../../extern/llvm/configure --target=arm-none-eabi --enable-interwork --prefix=$ROOT/toolchain
fi
echo "building LLVM+CLANG"
make
echo "installing LLVM+CLANG"
make install
cd $ROOT