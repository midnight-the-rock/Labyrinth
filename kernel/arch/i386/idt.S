//// a

.section .text

.extern	isr_handler
.extern interrupt_handler

.macro isr_entry num
isr_entry_\num:
	push	%ebp
	mov	%esp, %ebp
	push	$\num
	call	interrupt_handler
	leave
	iret
.endm	

.altmacro
.set	i, 0
.rept	256
isr_entry %i
.set	i, i + 1
.endr

.global isr_stub_table

isr_stub_table:	

.macro isr_entry_ptr num
	.long	isr_entry_\num
.endm

.altmacro
.set	i, 0
.rept 256
	isr_entry_ptr %i
.set	i, i + 1
.endr
