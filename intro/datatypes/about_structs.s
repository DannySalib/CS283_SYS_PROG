	.arch armv8-a
	.file	"about_structs.c"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB0:
	.cfi_startproc
	stp	x29, x30, [sp, -16]!
	.cfi_def_cfa_offset 16
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	mov	x29, sp
	bl	basic_struct_demo
	bl	type_struct_demo
	bl	pointer_struct_demo_strange
	bl	pointer_struct_demo
	mov	w0, 0
	ldp	x29, x30, [sp], 16
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.section	.rodata
	.align	3
.LC0:
	.string	"d1: f1:%d, f2:%d, f3:%d, f4:%ld\n"
	.align	3
.LC1:
	.string	"pd1 (ptr notation): f1:%d, f2:%d, f3:%d, f4:%ld\n"
	.align	3
.LC2:
	.string	"pd1 (arrow notation): f1:%d, f2:%d, f3:%d, f4:%ld\n"
	.text
	.align	2
	.global	basic_struct_demo
	.type	basic_struct_demo, %function
basic_struct_demo:
.LFB1:
	.cfi_startproc
	sub	sp, sp, #48
	.cfi_def_cfa_offset 48
	stp	x29, x30, [sp, 32]
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	add	x29, sp, 32
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x1, [x0]
	str	x1, [sp, 24]
	mov	x1, 0
	mov	w0, 1
	strb	w0, [sp, 8]
	mov	w0, 2
	strh	w0, [sp, 10]
	mov	w0, 3
	str	w0, [sp, 12]
	mov	x0, 4
	str	x0, [sp, 16]
	ldrsb	w0, [sp, 8]
	mov	w5, w0
	ldrsh	w0, [sp, 10]
	mov	w2, w0
	ldr	w0, [sp, 12]
	ldr	x1, [sp, 16]
	mov	x4, x1
	mov	w3, w0
	mov	w1, w5
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	printf
	add	x0, sp, 8
	str	x0, [sp]
	ldr	x0, [sp]
	ldrsb	w0, [x0]
	mov	w5, w0
	ldr	x0, [sp]
	ldrsh	w0, [x0, 2]
	mov	w2, w0
	ldr	x0, [sp]
	ldr	w1, [x0, 4]
	ldr	x0, [sp]
	ldr	x0, [x0, 8]
	mov	x4, x0
	mov	w3, w1
	mov	w1, w5
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	bl	printf
	ldr	x0, [sp]
	ldrsb	w0, [x0]
	mov	w5, w0
	ldr	x0, [sp]
	ldrsh	w0, [x0, 2]
	mov	w2, w0
	ldr	x0, [sp]
	ldr	w1, [x0, 4]
	ldr	x0, [sp]
	ldr	x0, [x0, 8]
	mov	x4, x0
	mov	w3, w1
	mov	w1, w5
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
	bl	printf
	mov	w0, 10
	bl	putchar
	nop
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x2, [sp, 24]
	ldr	x1, [x0]
	subs	x2, x2, x1
	mov	x1, 0
	beq	.L4
	bl	__stack_chk_fail
.L4:
	ldp	x29, x30, [sp, 32]
	add	sp, sp, 48
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE1:
	.size	basic_struct_demo, .-basic_struct_demo
	.section	.rodata
	.align	3
.LC3:
	.string	"d2: f1:%d, f2:%d, f3:%d, f4:%ld\n"
	.align	3
.LC4:
	.string	"pd2 (ptr notation): f1:%d, f2:%d, f3:%d, f4:%ld\n"
	.align	3
.LC5:
	.string	"pd2 (arrow notation): f1:%d, f2:%d, f3:%d, f4:%ld\n"
	.text
	.align	2
	.global	type_struct_demo
	.type	type_struct_demo, %function
type_struct_demo:
.LFB2:
	.cfi_startproc
	sub	sp, sp, #48
	.cfi_def_cfa_offset 48
	stp	x29, x30, [sp, 32]
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	add	x29, sp, 32
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x1, [x0]
	str	x1, [sp, 24]
	mov	x1, 0
	mov	w0, 1
	strb	w0, [sp, 8]
	mov	w0, 2
	strh	w0, [sp, 10]
	mov	w0, 3
	str	w0, [sp, 12]
	mov	x0, 4
	str	x0, [sp, 16]
	ldrsb	w0, [sp, 8]
	mov	w5, w0
	ldrsh	w0, [sp, 10]
	mov	w2, w0
	ldr	w0, [sp, 12]
	ldr	x1, [sp, 16]
	mov	x4, x1
	mov	w3, w0
	mov	w1, w5
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	bl	printf
	add	x0, sp, 8
	str	x0, [sp]
	ldr	x0, [sp]
	ldrsb	w0, [x0]
	mov	w5, w0
	ldr	x0, [sp]
	ldrsh	w0, [x0, 2]
	mov	w2, w0
	ldr	x0, [sp]
	ldr	w1, [x0, 4]
	ldr	x0, [sp]
	ldr	x0, [x0, 8]
	mov	x4, x0
	mov	w3, w1
	mov	w1, w5
	adrp	x0, .LC4
	add	x0, x0, :lo12:.LC4
	bl	printf
	ldr	x0, [sp]
	ldrsb	w0, [x0]
	mov	w5, w0
	ldr	x0, [sp]
	ldrsh	w0, [x0, 2]
	mov	w2, w0
	ldr	x0, [sp]
	ldr	w1, [x0, 4]
	ldr	x0, [sp]
	ldr	x0, [x0, 8]
	mov	x4, x0
	mov	w3, w1
	mov	w1, w5
	adrp	x0, .LC5
	add	x0, x0, :lo12:.LC5
	bl	printf
	mov	w0, 10
	bl	putchar
	nop
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x2, [sp, 24]
	ldr	x1, [x0]
	subs	x2, x2, x1
	mov	x1, 0
	beq	.L6
	bl	__stack_chk_fail
.L6:
	ldp	x29, x30, [sp, 32]
	add	sp, sp, 48
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE2:
	.size	type_struct_demo, .-type_struct_demo
	.section	.rodata
	.align	3
.LC6:
	.string	"vd2_strange (pointer math): f1:%d, f2:%d, f3:%d, f4:%ld\n"
	.text
	.align	2
	.global	pointer_struct_demo_strange
	.type	pointer_struct_demo_strange, %function
pointer_struct_demo_strange:
.LFB3:
	.cfi_startproc
	sub	sp, sp, #80
	.cfi_def_cfa_offset 80
	stp	x29, x30, [sp, 64]
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	add	x29, sp, 64
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x1, [x0]
	str	x1, [sp, 56]
	mov	x1, 0
	mov	w0, 1
	strb	w0, [sp, 40]
	mov	w0, 2
	strh	w0, [sp, 42]
	mov	w0, 3
	str	w0, [sp, 44]
	mov	x0, 4
	str	x0, [sp, 48]
	add	x0, sp, 40
	str	x0, [sp]
	ldr	x0, [sp]
	str	x0, [sp, 8]
	ldr	x0, [sp]
	add	x0, x0, 8
	str	x0, [sp, 16]
	ldr	x0, [sp]
	add	x0, x0, 24
	str	x0, [sp, 24]
	ldr	x0, [sp]
	add	x0, x0, 48
	str	x0, [sp, 32]
	ldr	x0, [sp, 8]
	ldrsb	w0, [x0]
	mov	w5, w0
	ldr	x0, [sp, 16]
	ldrsh	w0, [x0]
	mov	w2, w0
	ldr	x0, [sp, 24]
	ldr	w1, [x0]
	ldr	x0, [sp, 32]
	ldr	x0, [x0]
	mov	x4, x0
	mov	w3, w1
	mov	w1, w5
	adrp	x0, .LC6
	add	x0, x0, :lo12:.LC6
	bl	printf
	mov	w0, 10
	bl	putchar
	nop
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x2, [sp, 56]
	ldr	x1, [x0]
	subs	x2, x2, x1
	mov	x1, 0
	beq	.L8
	bl	__stack_chk_fail
.L8:
	ldp	x29, x30, [sp, 64]
	add	sp, sp, 80
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE3:
	.size	pointer_struct_demo_strange, .-pointer_struct_demo_strange
	.section	.rodata
	.align	3
.LC7:
	.string	"vd2 (pointer math): f1:%d, f2:%d, f3:%d, f4:%ld\n"
	.text
	.align	2
	.global	pointer_struct_demo
	.type	pointer_struct_demo, %function
pointer_struct_demo:
.LFB4:
	.cfi_startproc
	sub	sp, sp, #80
	.cfi_def_cfa_offset 80
	stp	x29, x30, [sp, 64]
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	add	x29, sp, 64
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x1, [x0]
	str	x1, [sp, 56]
	mov	x1, 0
	mov	w0, 1
	strb	w0, [sp, 40]
	mov	w0, 2
	strh	w0, [sp, 42]
	mov	w0, 3
	str	w0, [sp, 44]
	mov	x0, 4
	str	x0, [sp, 48]
	add	x0, sp, 40
	str	x0, [sp]
	ldr	x0, [sp]
	str	x0, [sp, 8]
	ldr	x0, [sp]
	add	x0, x0, 56
	str	x0, [sp, 16]
	ldr	x0, [sp]
	add	x0, x0, 32
	str	x0, [sp, 24]
	ldr	x0, [sp]
	add	x0, x0, 64
	str	x0, [sp, 32]
	ldr	x0, [sp, 8]
	ldrsb	w0, [x0]
	mov	w5, w0
	ldr	x0, [sp, 16]
	ldrsh	w0, [x0]
	mov	w2, w0
	ldr	x0, [sp, 24]
	ldr	w1, [x0]
	ldr	x0, [sp, 32]
	ldr	x0, [x0]
	mov	x4, x0
	mov	w3, w1
	mov	w1, w5
	adrp	x0, .LC7
	add	x0, x0, :lo12:.LC7
	bl	printf
	mov	w0, 10
	bl	putchar
	nop
	adrp	x0, :got:__stack_chk_guard
	ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x2, [sp, 56]
	ldr	x1, [x0]
	subs	x2, x2, x1
	mov	x1, 0
	beq	.L10
	bl	__stack_chk_fail
.L10:
	ldp	x29, x30, [sp, 64]
	add	sp, sp, 80
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE4:
	.size	pointer_struct_demo, .-pointer_struct_demo
	.ident	"GCC: (Ubuntu 13.2.0-23ubuntu4) 13.2.0"
	.section	.note.GNU-stack,"",@progbits
