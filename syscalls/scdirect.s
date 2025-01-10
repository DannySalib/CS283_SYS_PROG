	.arch armv8-a
	.file	"scdirect.c"
	.text
	.section	.rodata
	.align	3
.LC0:
	.string	"Hello, portable syscall!\n"
	.align	3
.LC1:
	.string	"syscall failed"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB0:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	mov	x29, sp
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	str	x0, [sp, 16]
	mov	w3, 25
	ldr	x2, [sp, 16]
	mov	w1, 1
	mov	x0, 64
	bl	syscall
	str	x0, [sp, 24]
	ldr	x0, [sp, 24]
	cmn	x0, #1
	bne	.L2
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	bl	perror
	mov	w0, 1
	b	.L3
.L2:
	mov	w0, 0
.L3:
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0"
	.section	.note.GNU-stack,"",@progbits
