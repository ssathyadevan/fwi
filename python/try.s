	.file	"try.cpp"
	.section	.text.unlikely,"ax",@progbits
.LCOLDB3:
	.section	.text.startup,"ax",@progbits
.LHOTB3:
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB2554:
	.cfi_startproc
	.cfi_personality 0x3,__gxx_personality_v0
	.cfi_lsda 0x3,.LLSDA2554
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	subq	$136, %rsp
	.cfi_def_cfa_offset 192
	leaq	64(%rsp), %rdi
	movq	%fs:40, %rax
	movq	%rax, 120(%rsp)
	xorl	%eax, %eax
.LEHB0:
	call	_ZN10ProfileCpuC1Ev
.LEHE0:
	movl	$512, %edi
.LEHB1:
	call	_Znam
	movl	$16384, %edi
	movq	%rax, %r15
	call	_Znam
	movl	$16384, %edi
	movq	%rax, %rbp
	call	_Znam
	movl	$1024, %edi
	call	_Znam
	leaq	1024(%rax), %rbx
	movq	%rax, %r12
.L2:
	movq	$0, (%rax)
	movq	$0, 8(%rax)
	addq	$16, %rax
	cmpq	%rbx, %rax
	jne	.L2
	leaq	512(%r15), %r13
	movq	%r15, %r14
.L4:
	movl	$32768, %edi
	call	_Znam
.LEHE1:
	leaq	32768(%rax), %rcx
	movq	%rax, %rdx
.L3:
	movq	$0, (%rdx)
	movq	$0, 8(%rdx)
	addq	$16, %rdx
	cmpq	%rdx, %rcx
	jne	.L3
	movq	%rax, (%r14)
	addq	$8, %r14
	cmpq	%r14, %r13
	jne	.L4
	movq	%r15, %rdx
.L6:
	movq	(%rdx), %rax
	movsd	.LC1(%rip), %xmm5
	leaq	32768(%rax), %rcx
.L5:
	movsd	%xmm5, (%rax)
	movq	$0, 8(%rax)
	addq	$16, %rax
	cmpq	%rax, %rcx
	jne	.L5
	addq	$8, %rdx
	cmpq	%r13, %rdx
	jne	.L6
	testb	$8, %bpl
	je	.L39
	movsd	%xmm5, 0(%rbp)
	movl	$1, %eax
	movl	$1, %ecx
	movl	$2047, %edi
	movl	$1023, %r8d
	movl	$2046, %edx
.L7:
	leaq	0(%rbp,%rcx,8), %rsi
	xorl	%ecx, %ecx
	movapd	.LC2(%rip), %xmm0
.L8:
	addl	$1, %ecx
	addq	$16, %rsi
	movaps	%xmm0, -16(%rsi)
	cmpl	%ecx, %r8d
	ja	.L8
	addl	%edx, %eax
	cmpl	%edi, %edx
	je	.L9
	movsd	.LC1(%rip), %xmm7
	cltq
	movsd	%xmm7, 0(%rbp,%rax,8)
.L9:
	leaq	48(%rsp), %rax
	movl	$29300, %edx
	leaq	32(%rsp), %rsi
	leaq	64(%rsp), %rdi
	movw	%dx, 48(%rsp)
	movb	$121, 2(%rax)
	movq	%rax, 32(%rsp)
	movq	$3, 40(%rsp)
	movb	$0, 51(%rsp)
.LEHB2:
	call	_ZN10ProfileCpu11StartRegionERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
.LEHE2:
	movq	32(%rsp), %rdi
	leaq	48(%rsp), %rax
	cmpq	%rax, %rdi
	je	.L10
	call	_ZdlPv
.L10:
	movl	$32768, %edi
.LEHB3:
	call	_Znam
	leaq	32768(%rax), %rdx
	movq	%rax, %r11
.L11:
	movq	$0, (%rax)
	movq	$0, 8(%rax)
	addq	$16, %rax
	cmpq	%rdx, %rax
	jne	.L11
	leaq	1024(%r11), %r14
	movq	%r11, %rax
.L12:
	movq	$0, (%rax)
	movq	$0, 8(%rax)
	addq	$16, %rax
	cmpq	%r14, %rax
	jne	.L12
	pxor	%xmm4, %xmm4
	movq	%rbp, (%rsp)
	xorl	%r9d, %r9d
	xorl	%r13d, %r13d
	movq	%rbp, 24(%rsp)
.L15:
	testl	%r13d, %r13d
	movl	%r13d, %eax
	movq	(%rsp), %rcx
	setne	23(%rsp)
	sall	$7, %eax
	leal	(%r13,%r13), %r10d
	movl	%eax, 12(%rsp)
	leal	32(%r13), %eax
	movq	%r11, %rbp
	movl	%eax, 16(%rsp)
.L13:
	movq	%rbp, %rsi
	movq	%r12, %rdi
	movq	%r15, %r8
	.p2align 4,,10
	.p2align 3
.L17:
	movq	%r9, %rdx
	addq	(%r8), %rdx
	xorl	%eax, %eax
	movapd	%xmm4, %xmm2
	movapd	%xmm4, %xmm1
	.p2align 4,,10
	.p2align 3
.L16:
	movsd	(%rcx,%rax), %xmm0
	movsd	(%rdx,%rax,2), %xmm3
	mulsd	%xmm0, %xmm3
	mulsd	8(%rdx,%rax,2), %xmm0
	addq	$8, %rax
	cmpq	$512, %rax
	addsd	%xmm3, %xmm2
	addsd	%xmm0, %xmm1
	jne	.L16
	movsd	%xmm2, (%rdi)
	addsd	(%rsi), %xmm2
	addq	$16, %rdi
	movsd	%xmm1, -8(%rdi)
	addsd	8(%rsi), %xmm1
	addq	$8, %r8
	addq	$16, %rsi
	movsd	%xmm2, -16(%rsi)
	movsd	%xmm1, -8(%rsi)
	cmpq	%rdi, %rbx
	jne	.L17
	cmpl	$31, %r10d
	jg	.L21
	cmpb	$0, 23(%rsp)
	je	.L21
	movslq	12(%rsp), %rdx
	xorl	%eax, %eax
	salq	$4, %rdx
	addq	%r11, %rdx
.L22:
	movupd	(%rdx,%rax), %xmm1
	movupd	(%r12,%rax), %xmm0
	addpd	%xmm1, %xmm0
	movups	%xmm0, (%rdx,%rax)
	addq	$16, %rax
	cmpq	$1024, %rax
	jne	.L22
.L21:
	addl	$1, %r10d
	addl	$64, 12(%rsp)
	addq	$512, %rcx
	addq	$1024, %rbp
	cmpl	%r10d, 16(%rsp)
	jne	.L13
	addl	$1, %r13d
	addq	$1024, %r9
	addq	$512, (%rsp)
	cmpl	$32, %r13d
	jne	.L15
	movq	24(%rsp), %rbp
	movl	$1024, %edi
	movl	$1, %r8d
	movl	$32, %r11d
	movl	$31, %r10d
	leaq	512(%rbp), %r9
.L28:
	movl	%r11d, %eax
	subl	%r8d, %eax
	cmpl	%r8d, %eax
	jl	.L23
	movq	%rbp, %rcx
	movq	%r14, %rsi
.L26:
	leaq	1024(%rsi), %r12
	movq	%r15, %rbx
.L25:
	movq	%rdi, %rdx
	addq	(%rbx), %rdx
	xorl	%eax, %eax
	movsd	(%rsi), %xmm2
	movsd	8(%rsi), %xmm1
	.p2align 4,,10
	.p2align 3
.L24:
	movsd	(%rcx,%rax), %xmm0
	movsd	(%rdx,%rax,2), %xmm3
	mulsd	%xmm0, %xmm3
	mulsd	8(%rdx,%rax,2), %xmm0
	addq	$8, %rax
	cmpq	$512, %rax
	addsd	%xmm3, %xmm2
	addsd	%xmm0, %xmm1
	jne	.L24
	movsd	%xmm2, (%rsi)
	addq	$16, %rsi
	addq	$8, %rbx
	movsd	%xmm1, -8(%rsi)
	cmpq	%r12, %rsi
	jne	.L25
	addq	$512, %rcx
	cmpq	%rcx, %r9
	jne	.L26
.L31:
	addl	$1, %r8d
	addq	$512, %r9
	addq	$1024, %rdi
	addq	$1024, %r14
	cmpl	$32, %r8d
	jne	.L28
	leaq	64(%rsp), %rdi
	call	_ZN10ProfileCpu9EndRegionEv
.LEHE3:
	movq	88(%rsp), %rdi
	leaq	104(%rsp), %rax
	movq	$_ZTV10ProfileCpu+16, 64(%rsp)
	cmpq	%rax, %rdi
	je	.L33
	call	_ZdlPv
.L33:
	xorl	%eax, %eax
	movq	120(%rsp), %rbx
	xorq	%fs:40, %rbx
	jne	.L71
	addq	$136, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
.L40:
	.cfi_restore_state
	movq	%rax, %rbx
.L36:
	movq	88(%rsp), %rdi
	leaq	104(%rsp), %rdx
	movq	$_ZTV10ProfileCpu+16, 64(%rsp)
	cmpq	%rdx, %rdi
	je	.L37
	call	_ZdlPv
.L37:
	movq	%rbx, %rdi
.LEHB4:
	call	_Unwind_Resume
.LEHE4:
.L39:
	xorl	%eax, %eax
	xorl	%ecx, %ecx
	movl	$2048, %edi
	movl	$1024, %r8d
	movl	$2048, %edx
	jmp	.L7
.L41:
	movq	32(%rsp), %rdi
	leaq	48(%rsp), %rdx
	movq	%rax, %rbx
	cmpq	%rdx, %rdi
	je	.L36
	call	_ZdlPv
	jmp	.L36
.L23:
	movl	%r10d, %r12d
	movq	%rbp, %rcx
	movq	%r14, %rsi
	subl	%r8d, %r12d
	addq	$1, %r12
	salq	$9, %r12
	addq	%rbp, %r12
.L32:
	leaq	1024(%rsi), %r13
	movq	%r15, %rbx
.L30:
	movq	%rdi, %rdx
	addq	(%rbx), %rdx
	xorl	%eax, %eax
	movsd	(%rsi), %xmm2
	movsd	8(%rsi), %xmm1
	.p2align 4,,10
	.p2align 3
.L29:
	movsd	(%rcx,%rax), %xmm0
	movsd	(%rdx,%rax,2), %xmm3
	mulsd	%xmm0, %xmm3
	mulsd	8(%rdx,%rax,2), %xmm0
	addq	$8, %rax
	cmpq	$512, %rax
	addsd	%xmm3, %xmm2
	addsd	%xmm0, %xmm1
	jne	.L29
	movsd	%xmm2, (%rsi)
	addq	$16, %rsi
	addq	$8, %rbx
	movsd	%xmm1, -8(%rsi)
	cmpq	%rsi, %r13
	jne	.L30
	addq	$512, %rcx
	cmpq	%rcx, %r12
	jne	.L32
	jmp	.L31
.L71:
	call	__stack_chk_fail
	.cfi_endproc
.LFE2554:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA2554:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2554-.LLSDACSB2554
.LLSDACSB2554:
	.uleb128 .LEHB0-.LFB2554
	.uleb128 .LEHE0-.LEHB0
	.uleb128 0
	.uleb128 0
	.uleb128 .LEHB1-.LFB2554
	.uleb128 .LEHE1-.LEHB1
	.uleb128 .L40-.LFB2554
	.uleb128 0
	.uleb128 .LEHB2-.LFB2554
	.uleb128 .LEHE2-.LEHB2
	.uleb128 .L41-.LFB2554
	.uleb128 0
	.uleb128 .LEHB3-.LFB2554
	.uleb128 .LEHE3-.LEHB3
	.uleb128 .L40-.LFB2554
	.uleb128 0
	.uleb128 .LEHB4-.LFB2554
	.uleb128 .LEHE4-.LEHB4
	.uleb128 0
	.uleb128 0
.LLSDACSE2554:
	.section	.text.startup
	.size	main, .-main
	.section	.text.unlikely
.LCOLDE3:
	.section	.text.startup
.LHOTE3:
	.section	.text.unlikely
.LCOLDB4:
	.section	.text.startup
.LHOTB4:
	.p2align 4,,15
	.type	_GLOBAL__sub_I_main, @function
_GLOBAL__sub_I_main:
.LFB2753:
	.cfi_startproc
	subq	$8, %rsp
	.cfi_def_cfa_offset 16
	movl	$_ZStL8__ioinit, %edi
	call	_ZNSt8ios_base4InitC1Ev
	movl	$__dso_handle, %edx
	movl	$_ZStL8__ioinit, %esi
	movl	$_ZNSt8ios_base4InitD1Ev, %edi
	addq	$8, %rsp
	.cfi_def_cfa_offset 8
	jmp	__cxa_atexit
	.cfi_endproc
.LFE2753:
	.size	_GLOBAL__sub_I_main, .-_GLOBAL__sub_I_main
	.section	.text.unlikely
.LCOLDE4:
	.section	.text.startup
.LHOTE4:
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I_main
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	0
	.long	1072693248
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC2:
	.long	0
	.long	1072693248
	.long	0
	.long	1072693248
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
