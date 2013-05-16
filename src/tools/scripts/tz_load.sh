#!/bin/sh

echo "Loading Kernel module...\n"
sh scripts/tz_tee.kld
echo "Kernel module loaded\n"

./norm/daemons/tzsd.bin &

LD_LIBRARY_PATH=$PWD/norm/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH