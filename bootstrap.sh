#!/bin/sh

echo "========================================"
echo "Git Submodule initialization"
echo "========================================"

#git submodule init
#git submodule update

mkdir -p toolchain
mkdir -p qemu
mkdir -p build/qemu
mkdir -p build/binutils
mkdir -p build/llvm

echo "========================================"
echo "Building tz qemu"
echo "========================================"

./script/qemu_build.sh

echo "========================================"
echo "Building bare metal binutils"
echo "========================================"

./script/binutils_build.sh


echo "========================================"
echo "Building bare metal LLVM+CLANG"
echo "========================================"

./script/llvm_build.sh


