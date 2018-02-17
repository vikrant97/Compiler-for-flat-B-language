	.text
	.file	"llout"
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:                                 # %B1
	pushq	%rax
.Ltmp0:
	.cfi_def_cfa_offset 16
	movl	$1, i(%rip)
	xorl	%eax, %eax
	testb	%al, %al
	jne	.LBB0_2
	.align	16, 0x90
.LBB0_1:                                # %for
                                        # =>This Inner Loop Header: Depth=1
	movslq	i(%rip), %rax
	movl	%eax, data(,%rax,4)
	movl	i(%rip), %eax
	incl	%eax
	movl	%eax, i(%rip)
	cmpl	$101, %eax
	jb	.LBB0_1
.LBB0_2:                                # %next
	movl	$0, sum(%rip)
	movl	$1, i(%rip)
	xorl	%eax, %eax
	testb	%al, %al
	jne	.LBB0_4
	.align	16, 0x90
.LBB0_3:                                # %for2
                                        # =>This Inner Loop Header: Depth=1
	movslq	i(%rip), %rax
	movl	data(,%rax,4), %eax
	addl	%eax, sum(%rip)
	movl	i(%rip), %eax
	incl	%eax
	movl	%eax, i(%rip)
	cmpl	$101, %eax
	jb	.LBB0_3
.LBB0_4:                                # %next3
	movl	$.L__unnamed_1, %edi
	movl	$.L__unnamed_2, %esi
	xorl	%eax, %eax
	callq	printf
	movl	sum(%rip), %esi
	movl	$.L__unnamed_3, %edi
	xorl	%eax, %eax
	callq	printf
	movl	$.L__unnamed_4, %edi
	movl	$.L__unnamed_5, %esi
	xorl	%eax, %eax
	callq	printf
	popq	%rax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	data,@object            # @data
	.bss
	.globl	data
	.align	16
data:
	.zero	400
	.size	data, 400

	.type	i,@object               # @i
	.comm	i,4,4
	.type	sum,@object             # @sum
	.comm	sum,4,4
	.type	.L__unnamed_2,@object   # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_2:
	.asciz	"Sum value: "
	.size	.L__unnamed_2, 12

	.type	.L__unnamed_1,@object   # @1
.L__unnamed_1:
	.asciz	"%s "
	.size	.L__unnamed_1, 4

	.type	.L__unnamed_3,@object   # @2
.L__unnamed_3:
	.asciz	"%d "
	.size	.L__unnamed_3, 4

	.type	.L__unnamed_5,@object   # @3
.L__unnamed_5:
	.asciz	"\n"
	.size	.L__unnamed_5, 2

	.type	.L__unnamed_4,@object   # @4
.L__unnamed_4:
	.asciz	"%s"
	.size	.L__unnamed_4, 3


	.section	".note.GNU-stack","",@progbits
