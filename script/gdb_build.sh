#!/bin/bash
. script/libbuild.sh
ROOT=$(pwd)
WHAT=gdb
CFGFLAGS="--target=arm-none-eabi --enable-interwork --prefix=$ROOT/toolchain"

cd_mk

check

std_build

