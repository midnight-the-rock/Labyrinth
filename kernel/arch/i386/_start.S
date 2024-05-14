//// prekernel stuff here

#define STACK_SIZE 4096
	
.code32
.section .text

.extern _init
.extern early_main

.global _start
.type	_start, @function

_start:
	movl	$(stack + STACK_SIZE), %esp 

	push	%ebx // multiboot info ptr
	push	%eax // multiboot magic
	call	_init
	call	early_main

	jmp .
	hlt

.section .bss	

.comm stack, STACK_SIZE
