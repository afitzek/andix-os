#!/bin/bash
echo "#!/bin/ash" > load.sh

echo "SERVER=192.168.200.1" >> load.sh
echo "PPATH=andix" >> load.sh

find ./ -mindepth 1 -maxdepth 1 -type d | sed 's|./||' | sed 's|^|rm -rf |' >> load.sh
find ./ -mindepth 1 -type d | sed 's|./||' | sed 's|^|mkdir -p |' >> load.sh
find ./ -type f | sed 's|./||' | awk '{print "wget http://192.168.200.1/andix/"$1" -O"$1"\0"}' >> load.sh

echo "chmod -R +x *.sh" >> load.sh
echo "chmod -R +x *.kld" >> load.sh
echo "chmod -R +x *.kuld" >> load.sh
echo "chmod -R +x *.bin" >> load.sh
