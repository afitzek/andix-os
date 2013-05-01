#!/bin/bash
echo "#!/bin/ash" > load.sh

echo "SERVER=192.168.200.1" >> load.sh
echo "PPATH=andix" >> load.sh

find ./ -mindepth 1 -maxdepth 1 -type d | sed 's|./||' | sed 's|^|rm -rf |' >> load.sh
find ./ -mindepth 1 -type d | sed 's|./||' | sed 's|^|mkdir -p |' >> load.sh
find ./ -type f | sed 's|./||' | awk '{print "wget http://192.168.200.1/andix/"$1" -O"$1"\0"}' >> load.sh

echo "find ./ -type f -name '*.sh' -exec chmod 555 {} \;" >> load.sh
echo "find ./ -type f -name '*.kld' -exec chmod 555 {} \;" >> load.sh
echo "find ./ -type f -name '*.kuld' -exec chmod 555 {} \;" >> load.sh
echo "find ./ -type f -name '*.bin' -exec chmod 555 {} \;" >> load.sh
