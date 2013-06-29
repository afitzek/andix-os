OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")

SECTIONS
{
	start = .; _start = .; __start = .;
	#ifdef QEMU
	.text 0xC0010000 : AT(0xB0010000) {
	#else
	.text 0xC0008000 : AT(0xB0008000) {
	#endif
		code = .; _code = .; __code = .;
		*(.reset)
		*(.text)
		code_end = .; _code_end = .; __code_end = .;
	}
	.rodata : {
		rodata = .; _rodata = .; __rodata = .;
		*(.rodata)
		rodata_end = .; _rodata_end = .; __rodata_end = .;
		__symbol_table = .;
       *(.symbol_table)
       __symbol_table_end = .;
	}
	.hal ALIGN(4) : {
		. = ALIGN(4);
		__hal_platform_table = .;
    	*(.hal.platform.ptr); 
    	__hal_platform_table_end = .;
    	. = ALIGN(4);
		__hal_driver_table = .;
    	*(.hal.platform.driver); 
    	__hal_driver_table_end = .;
    	. = ALIGN(4);
    	userpayload = .; _userpayload = .; __userpayload = .;
		*(.usrpayload);
		userpayload_end = .; _userpayload_end = .; __userpayload_end = .;
		. = ALIGN(4);
		samplepayload = .; _samplepayload = .; __samplepayload = .;
		*(.samppayload);
		samplepayload_end = .; _samplepayload_end = .; __samplepayload_end = .;
    	. = ALIGN(4);
    	payload = .; _payload = .; __payload = .;
		*(.payload);
		payload_end = .; _payload_end = .; __payload_end = .;
		. = ALIGN(4);
    	payload_and = .; _payload_and = .; __payload_and = .;
		*(.payload_and);
		payload_and_end = .; _payload_and_end = .; __payload_and_end = .;
		. = ALIGN(4);
    	payload_and_rd = .; _payload_and_rd = .; __payload_and_rd = .;
		*(.payload_and_rd);
		payload_and_rd_end = .; _payload_and_rd_end = .; __payload_and_rd_end = .;
		. = ALIGN(4);
		dtb = .; _dtb = .; __dtb = .;
		*(.dtb);
		dtb_end = .; _dtb_end = .; __dtb_end = .;
	}
	.data ALIGN(1M) : {
		data = .; _data = .; __data = .;
		*(.data)
		data_end = .; _data_end = .; __data_end = .;
	}
	.bss ALIGN(1M) : {
		bss = .; _bss = .; __bss = .;
		*(.bss)
		bss_end = .; _bss_end = .; __bss_end = .;
	}
	.mmu ALIGN(1M) : {
		mmu = .; _mmu = .; __mmu = .;
		*(.mmu)
		mmu_end = .; _mmu_end = .; __mmu_end = .;
	}
	__symbol_table_start__ = .;
    .symbol_table ALIGN(1M) :
    {
       
    }
    __symbol_table_end__ = __symbol_table_start__ + SIZEOF(.symbol_table);
	end = .; _end = .; __end = .;
}