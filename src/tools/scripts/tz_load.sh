#!/bin/sh

echo "Loading Kernel modules...\n"
sh tz_tee.kld
sh tz_ctrl.kld
echo "Kernel modules loaded\n"