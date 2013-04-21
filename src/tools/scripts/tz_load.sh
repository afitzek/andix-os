#!/bin/sh

echo "Loading Kernel modules...\n"
sh scripts/tz_tee.kld
sh scripts/tz_ctrl.kld
echo "Kernel modules loaded\n"