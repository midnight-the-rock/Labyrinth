ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)

OUTPUT(kernel.bin)

SECTIONS {
   	. = 1M; /* get the kernel to load at 1M offset (0x100000) */

	.multiboot ALIGN(1M) : {
	     *(.multiboot)
	}

	.init ALIGN(1M) : {
	     *(.init)
	}

	.text ALIGN(1M) : {
	     *(.text)
	}

	.fini ALIGN(1M) : {
	     *(.fini)
	}

	.rodata ALIGN(1M) : {
	     *(.rodata)
	}

	.data ALIGN(1M) : {
	     *(.data)
	}	

	.bss ALIGN(1M) : {
	     *(COMMON)
	     *(.bss)
	}

	end_of_kernel = ALIGN(1M);
}
