#!/bin/sh

killall tzsd.bin

echo "Unloading Kernel modules...\n"
sh scripts/tz_tee.kuld
echo "Kernel modules unloaded\n"

