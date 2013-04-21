#!/bin/ash
SERVER=192.168.200.1
PPATH=andix
wget http://192.168.200.1/andix/.project -O=.project 
wget http://192.168.200.1/andix/load.sh -O=load.sh 
wget http://192.168.200.1/andix/tz_ctrl.kld -O=tz_ctrl.kld 
wget http://192.168.200.1/andix/tz_unload.sh -O=tz_unload.sh 
wget http://192.168.200.1/andix/buildload.sh -O=buildload.sh 
wget http://192.168.200.1/andix/.cproject -O=.cproject 
wget http://192.168.200.1/andix/tz_tee.kuld -O=tz_tee.kuld 
wget http://192.168.200.1/andix/Makefile -O=Makefile 
wget http://192.168.200.1/andix/tz_ctrl.kuld -O=tz_ctrl.kuld 
wget http://192.168.200.1/andix/tz_load.sh -O=tz_load.sh 
wget http://192.168.200.1/andix/tz_tee.kld -O=tz_tee.kld 
chmod -R +x *.sh
chmod -R +x *.kld
chmod -R +x *.kuld
chmod -R +x *.bin
