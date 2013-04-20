#!/bin/sh

echo "Unloading Kernel modules...\n"
sh tz_tee.kuld
sh tz_ctrl.kuld
echo "Kernel modules unloaded\n"