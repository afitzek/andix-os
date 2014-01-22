#!/bin/bash

ROOT=$(pwd)
WHAT=binutils
. script/libbuild.sh
CFGFLAGS="--target=arm-none-eabi --prefix=$ROOT/toolchain"

cd_mk

check

std_build



