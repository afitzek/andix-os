#!/bin/sh

echo "Unloading Kernel modules...\n"
sh scripts/tz_tee.kuld
sh scripts/tz_ctrl.kuld
echo "Kernel modules unloaded\n"