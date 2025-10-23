	.file	"grade.c"
	.text
	.section	.rodata
.LC0:
	.string	"Enter Marks: "
.LC1:
	.string	"%i"
.LC2:
	.string	"Grade = E"
.LC3:
	.string	"Grade = C"
.LC4:
	.string	"Grade = B"
.LC5:
	.string	"Grade = A"
.LC6:
	.string	"Enter VALID MARKS"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	leaq	-12(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	__isoc99_scanf@PLT
	movl	-12(%rbp), %eax
	testl	%eax, %eax
	js	.L2
	movl	-12(%rbp), %eax
	cmpl	$49, %eax
	jg	.L2
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L3
.L2:
	movl	-12(%rbp), %eax
	cmpl	$49, %eax
	jle	.L4
	movl	-12(%rbp), %eax
	cmpl	$65, %eax
	jg	.L4
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L3
.L4:
	movl	-12(%rbp), %eax
	cmpl	$65, %eax
	jle	.L5
	movl	-12(%rbp), %eax
	cmpl	$75, %eax
	jg	.L5
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L3
.L5:
	movl	-12(%rbp), %eax
	cmpl	$75, %eax
	jle	.L6
	movl	-12(%rbp), %eax
	cmpl	$100, %eax
	jg	.L6
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	jmp	.L3
.L6:
	leaq	.LC6(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
.L3:
	movl	$0, %eax
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L8
	call	__stack_chk_fail@PLT
.L8:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
