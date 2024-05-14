/* function to be used once the gdt has been loaded
   and the CS register has to be updated */

.code32
.section .text

.global	reload_cs
.type	reload_cs, @function

reload_cs:	
	push	$0x08
	push	$flush_gdt
	retf

flush_gdt:	
	mov	$0x10, %ax
	mov	%ax, %ds
	mov	%ax, %es
	mov	%ax, %fs
	mov	%ax, %gs
	mov	%ax, %ss

	ret
