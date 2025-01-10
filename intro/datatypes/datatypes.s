	.arch armv8-a
	.file	"datatypes.c"
	.text
	.section	.rodata
	.align	3
.LC0:
	.string	"The size of char is %ld\n"
	.align	3
.LC1:
	.string	"The size of int is %ld\n"
	.align	3
.LC2:
	.string	"The size of long is %ld\n"
	.align	3
.LC3:
	.string	"The value of char is %c\n"
	.align	3
.LC4:
	.string	"The value of int is %d\n"
	.align	3
.LC5:
	.string	"The value of long is %ld\n"
	.align	3
.LC6:
	.string	"The value of long is %lld\n"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB0:
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x29, sp
	mov	x1, 1
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	printf
	mov	x1, 4
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	bl	printf
	mov	x1, 8
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
	bl	printf
	mov	x1, 8
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
	bl	printf
	ldrb	w0, [sp, 27]
	mov	w1, w0
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	bl	printf
	ldr	w1, [sp, 28]
	adrp	x0, .LC4
	add	x0, x0, :lo12:.LC4
	bl	printf
	ldr	x1, [sp, 32]
	adrp	x0, .LC5
	add	x0, x0, :lo12:.LC5
	bl	printf
	ldr	x1, [sp, 40]
	adrp	x0, .LC6
	add	x0, x0, :lo12:.LC6
	bl	printf
	bl	sysprog_types
	mov	w0, 0
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align	3
.LC7:
	.string	"The size of uint8_t is %ld\n"
	.align	3
.LC8:
	.string	"The size of uint16_t is %ld\n"
	.align	3
.LC9:
	.string	"The size of uint32_t is %ld\n"
	.align	3
.LC10:
	.string	"The size of uint64_t is %ld\n"
	.text
	.align	2
	.global	sysprog_types
	.type	sysprog_types, %function
sysprog_types:
.LFB1:
	.cfi_startproc
	stp	x29, x30, [sp, -16]!
	.cfi_def_cfa_offset 16
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	mov	x29, sp
	mov	w0, 10
	bl	putchar
	mov	x1, 1
	adrp	x0, .LC7
	add	x0, x0, :lo12:.LC7
	bl	printf
	mov	x1, 2
	adrp	x0, .LC8
	add	x0, x0, :lo12:.LC8
	bl	printf
	mov	x1, 4
	adrp	x0, .LC9
	add	x0, x0, :lo12:.LC9
	bl	printf
	mov	x1, 8
	adrp	x0, .LC10
	add	x0, x0, :lo12:.LC10
	bl	printf
	nop
	ldp	x29, x30, [sp], 16
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE1:
	.size	sysprog_types, .-sysprog_types
	.ident	"GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0"
	.section	.note.GNU-stack,"",@progbits
