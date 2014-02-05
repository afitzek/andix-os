J=$(cat /proc/cpuinfo | grep "^processor" | wc -l)

function cd_mk() 
{
	cd $ROOT &&
	mkdir -p build/$WHAT &&
	cd build/$WHAT
}

function std_build() 
{
	if [ ! -e Makefile ]; then
	  echo "configuring $WHAT"
	  $ROOT/extern/$WHAT/configure $CFGFLAGS
	fi &&
	echo "building $WHAT" &&
	make  -j$J &&
	echo "installing $WHAT" &&
	make install 
	cd $ROOT
}

function check() {
	echo $ROOT $WHAT
	echo $CFGFLAGS
}