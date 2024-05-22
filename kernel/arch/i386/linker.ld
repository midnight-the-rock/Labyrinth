ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)

OUTPUT(kernel.bin)

SECTIONS {
   	. = 1M; /* get the kernel to load at 1M offset (0x100000) */

	.multiboot ALIGN(4K) : {
	     *(.multiboot)
	}

	.init ALIGN(4K) :  {
	     *(.init)
	}

	.text ALIGN(4K) :  {
	     *(.text)
	}

	.fini ALIGN(4K) :  {
	     *(.fini)
	}

	.rodata ALIGN(4K) : {
	     *(.rodata)
	}

	.data ALIGN(4K) :  {
	     *(.data)
	}	

	.bss ALIGN(4K) : {
	     *(COMMON)
	     *(.bss)
	}

	PROVIDE(end_of_kernel = .);
}
