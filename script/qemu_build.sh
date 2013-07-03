#!/bin/sh
ROOT=$(pwd)
echo "Building in $ROOT"
#git submodule update
cd ./extern/qemu-trustzone
git checkout for-linaro
make distclean && rm -rf *-linux-user *-softmmu
cd $ROOT
cd build/qemu
if [ ! -e Makefile ]; then
  echo "configuring QEMU"
  ../../extern/qemu-trustzone/configure --target-list=arm-softmmu --prefix=$ROOT/qemu
fi
echo "building QEMU"
make
echo "installing QEMU"
make install