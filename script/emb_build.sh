#!/bin/sh
EROOT=$(pwd | sed 's/\//\\\//g')
ROOT=$(pwd)
cd ./script
cp embtoolkit.config embtoolkit.config.fixed
sed -i -e "s/{A_ROOT}/$EROOT/" embtoolkit.config.fixed
grep {A_ROOT} embtoolkit.config.fixed
cd $ROOT/extern/embtoolkit.git
git checkout master
cp $ROOT/script/embtoolkit.config.fixed .config
echo "Building Embedded Toolkit"
make
cd $ROOT