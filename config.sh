export ANDIX_ROOT=$PWD
export ANDIX_DEPLOY=$ANDIX_ROOT/deploy
export LINUX_KERNEL=$ANDIX_ROOT/../linux-linaro-tracking
export ROOTFS=$ANDIX_ROOT/../linaro-userland/binary
TOOLCHAINROOT=$ANDIX_ROOT/toolchain
export PATH=$TOOLCHAINROOT/bin:$PATH


croot() 
{
	cd $ANDIX_ROOT
}

deploykernel()
{
	cp $LINUX_KERNEL/arch/arm/boot/zImage $ANDIX_ROOT/src/trusted/kernelspace/kernel/payload/norm_kernel.bin
}


