	.file	"ProfileCpu.cpp"
	.section	.text.unlikely._ZNKSt5ctypeIcE8do_widenEc,"axG",@progbits,_ZNKSt5ctypeIcE8do_widenEc,comdat
	.align 2
.LCOLDB0:
	.section	.text._ZNKSt5ctypeIcE8do_widenEc,"axG",@progbits,_ZNKSt5ctypeIcE8do_widenEc,comdat
.LHOTB0:
	.align 2
	.p2align 4,,15
	.weak	_ZNKSt5ctypeIcE8do_widenEc
	.type	_ZNKSt5ctypeIcE8do_widenEc, @function
_ZNKSt5ctypeIcE8do_widenEc:
.LFB1210:
	.cfi_startproc
	movl	%esi, %eax
	ret
	.cfi_endproc
.LFE1210:
	.size	_ZNKSt5ctypeIcE8do_widenEc, .-_ZNKSt5ctypeIcE8do_widenEc
	.section	.text.unlikely._ZNKSt5ctypeIcE8do_widenEc,"axG",@progbits,_ZNKSt5ctypeIcE8do_widenEc,comdat
.LCOLDE0:
	.section	.text._ZNKSt5ctypeIcE8do_widenEc,"axG",@progbits,_ZNKSt5ctypeIcE8do_widenEc,comdat
.LHOTE0:
	.section	.text.unlikely._ZN10ProfileCpuD2Ev,"axG",@progbits,_ZN10ProfileCpuD5Ev,comdat
	.align 2
.LCOLDB1:
	.section	.text._ZN10ProfileCpuD2Ev,"axG",@progbits,_ZN10ProfileCpuD5Ev,comdat
.LHOTB1:
	.align 2
	.p2align 4,,15
	.weak	_ZN10ProfileCpuD2Ev
	.type	_ZN10ProfileCpuD2Ev, @function
_ZN10ProfileCpuD2Ev:
.LFB2040:
	.cfi_startproc
	movq	%rdi, %rax
	movq	$_ZTV10ProfileCpu+16, (%rdi)
	movq	24(%rdi), %rdi
	addq	$40, %rax
	cmpq	%rax, %rdi
	je	.L2
	jmp	_ZdlPv
	.p2align 4,,10
	.p2align 3
.L2:
	rep ret
	.cfi_endproc
.LFE2040:
	.size	_ZN10ProfileCpuD2Ev, .-_ZN10ProfileCpuD2Ev
	.section	.text.unlikely._ZN10ProfileCpuD2Ev,"axG",@progbits,_ZN10ProfileCpuD5Ev,comdat
.LCOLDE1:
	.section	.text._ZN10ProfileCpuD2Ev,"axG",@progbits,_ZN10ProfileCpuD5Ev,comdat
.LHOTE1:
	.weak	_ZN10ProfileCpuD1Ev
	.set	_ZN10ProfileCpuD1Ev,_ZN10ProfileCpuD2Ev
	.section	.text.unlikely._ZN10ProfileCpuD0Ev,"axG",@progbits,_ZN10ProfileCpuD5Ev,comdat
	.align 2
.LCOLDB2:
	.section	.text._ZN10ProfileCpuD0Ev,"axG",@progbits,_ZN10ProfileCpuD5Ev,comdat
.LHOTB2:
	.align 2
	.p2align 4,,15
	.weak	_ZN10ProfileCpuD0Ev
	.type	_ZN10ProfileCpuD0Ev, @function
_ZN10ProfileCpuD0Ev:
.LFB2042:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	movq	$_ZTV10ProfileCpu+16, (%rdi)
	movq	24(%rdi), %rdi
	leaq	40(%rbx), %rax
	cmpq	%rax, %rdi
	je	.L6
	call	_ZdlPv
.L6:
	movq	%rbx, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 8
	jmp	_ZdlPv
	.cfi_endproc
.LFE2042:
	.size	_ZN10ProfileCpuD0Ev, .-_ZN10ProfileCpuD0Ev
	.section	.text.unlikely._ZN10ProfileCpuD0Ev,"axG",@progbits,_ZN10ProfileCpuD5Ev,comdat
.LCOLDE2:
	.section	.text._ZN10ProfileCpuD0Ev,"axG",@progbits,_ZN10ProfileCpuD5Ev,comdat
.LHOTE2:
	.section	.text.unlikely,"ax",@progbits
	.align 2
.LCOLDB3:
	.text
.LHOTB3:
	.align 2
	.p2align 4,,15
	.globl	_ZN10ProfileCpu11StartRegionERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	.type	_ZN10ProfileCpu11StartRegionERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE, @function
_ZN10ProfileCpu11StartRegionERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE:
.LFB2046:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	%rdi, %rbx
	leaq	24(%rdi), %rdi
	call	_ZNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEE9_M_assignERKS4_
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movq	%rax, 8(%rbx)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE2046:
	.size	_ZN10ProfileCpu11StartRegionERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE, .-_ZN10ProfileCpu11StartRegionERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	.section	.text.unlikely
.LCOLDE3:
	.text
.LHOTE3:
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC5:
	.string	"Computation time for "
.LC6:
	.string	" was "
.LC7:
	.string	" seconds"
	.section	.text.unlikely
	.align 2
.LCOLDB8:
	.text
.LHOTB8:
	.align 2
	.p2align 4,,15
	.globl	_ZN10ProfileCpu9EndRegionEv
	.type	_ZN10ProfileCpu9EndRegionEv, @function
_ZN10ProfileCpu9EndRegionEv:
.LFB2047:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	pushq	%rbx
	.cfi_def_cfa_offset 24
	.cfi_offset 3, -24
	movq	%rdi, %rbx
	subq	$24, %rsp
	.cfi_def_cfa_offset 48
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movq	%rax, 16(%rbx)
	subq	8(%rbx), %rax
	movl	$21, %edx
	pxor	%xmm0, %xmm0
	movl	$.LC5, %esi
	movl	$_ZSt4cout, %edi
	cvtsi2sdq	%rax, %xmm0
	divsd	.LC4(%rip), %xmm0
	movsd	%xmm0, 8(%rsp)
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	32(%rbx), %rdx
	movl	$_ZSt4cout, %edi
	movq	24(%rbx), %rsi
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movl	$5, %edx
	movl	$.LC6, %esi
	movq	%rax, %rdi
	movq	%rax, %rbx
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movsd	8(%rsp), %xmm0
	movq	%rbx, %rdi
	call	_ZNSo9_M_insertIdEERSoT_
	movl	$8, %edx
	movq	%rax, %rbx
	movl	$.LC7, %esi
	movq	%rax, %rdi
	call	_ZSt16__ostream_insertIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_PKS3_l
	movq	(%rbx), %rax
	movq	-24(%rax), %rax
	movq	240(%rbx,%rax), %rbp
	testq	%rbp, %rbp
	je	.L17
	cmpb	$0, 56(%rbp)
	je	.L12
	movsbl	67(%rbp), %esi
.L13:
	movq	%rbx, %rdi
	call	_ZNSo3putEc
	addq	$24, %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 24
	movq	%rax, %rdi
	popq	%rbx
	.cfi_def_cfa_offset 16
	popq	%rbp
	.cfi_def_cfa_offset 8
	jmp	_ZNSo5flushEv
	.p2align 4,,10
	.p2align 3
.L12:
	.cfi_restore_state
	movq	%rbp, %rdi
	call	_ZNKSt5ctypeIcE13_M_widen_initEv
	movq	0(%rbp), %rax
	movl	$10, %esi
	movq	48(%rax), %rax
	cmpq	$_ZNKSt5ctypeIcE8do_widenEc, %rax
	je	.L13
	movq	%rbp, %rdi
	call	*%rax
	movsbl	%al, %esi
	jmp	.L13
.L17:
	call	_ZSt16__throw_bad_castv
	.cfi_endproc
.LFE2047:
	.size	_ZN10ProfileCpu9EndRegionEv, .-_ZN10ProfileCpu9EndRegionEv
	.section	.text.unlikely
.LCOLDE8:
	.text
.LHOTE8:
	.section	.text.unlikely
	.align 2
.LCOLDB9:
	.text
.LHOTB9:
	.align 2
	.p2align 4,,15
	.globl	_ZN10ProfileCpuC2Ev
	.type	_ZN10ProfileCpuC2Ev, @function
_ZN10ProfileCpuC2Ev:
.LFB2044:
	.cfi_startproc
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset 3, -16
	movq	$_ZTV10ProfileCpu+16, (%rdi)
	movq	%rdi, %rbx
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movq	%rax, 8(%rbx)
	call	_ZNSt6chrono3_V212system_clock3nowEv
	movq	%rax, 16(%rbx)
	leaq	40(%rbx), %rax
	movq	$0, 32(%rbx)
	movb	$0, 40(%rbx)
	movq	%rax, 24(%rbx)
	popq	%rbx
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE2044:
	.size	_ZN10ProfileCpuC2Ev, .-_ZN10ProfileCpuC2Ev
	.section	.text.unlikely
.LCOLDE9:
	.text
.LHOTE9:
	.globl	_ZN10ProfileCpuC1Ev
	.set	_ZN10ProfileCpuC1Ev,_ZN10ProfileCpuC2Ev
	.section	.text.unlikely
.LCOLDB10:
	.section	.text.startup,"ax",@progbits
.LHOTB10:
	.p2align 4,,15
	.type	_GLOBAL__sub_I__ZN10ProfileCpuC2Ev, @function
_GLOBAL__sub_I__ZN10ProfileCpuC2Ev:
.LFB2267:
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
.LFE2267:
	.size	_GLOBAL__sub_I__ZN10ProfileCpuC2Ev, .-_GLOBAL__sub_I__ZN10ProfileCpuC2Ev
	.section	.text.unlikely
.LCOLDE10:
	.section	.text.startup
.LHOTE10:
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__ZN10ProfileCpuC2Ev
	.weak	_ZTS10ProfileCpu
	.section	.rodata._ZTS10ProfileCpu,"aG",@progbits,_ZTS10ProfileCpu,comdat
	.align 8
	.type	_ZTS10ProfileCpu, @object
	.size	_ZTS10ProfileCpu, 13
_ZTS10ProfileCpu:
	.string	"10ProfileCpu"
	.weak	_ZTI10ProfileCpu
	.section	.rodata._ZTI10ProfileCpu,"aG",@progbits,_ZTI10ProfileCpu,comdat
	.align 8
	.type	_ZTI10ProfileCpu, @object
	.size	_ZTI10ProfileCpu, 16
_ZTI10ProfileCpu:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTS10ProfileCpu
	.weak	_ZTV10ProfileCpu
	.section	.rodata._ZTV10ProfileCpu,"aG",@progbits,_ZTV10ProfileCpu,comdat
	.align 8
	.type	_ZTV10ProfileCpu, @object
	.size	_ZTV10ProfileCpu, 48
_ZTV10ProfileCpu:
	.quad	0
	.quad	_ZTI10ProfileCpu
	.quad	_ZN10ProfileCpu11StartRegionERKNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
	.quad	_ZN10ProfileCpu9EndRegionEv
	.quad	_ZN10ProfileCpuD1Ev
	.quad	_ZN10ProfileCpuD0Ev
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC4:
	.long	0
	.long	1104006501
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 5.4.0-6ubuntu1~16.04.4) 5.4.0 20160609"
	.section	.note.GNU-stack,"",@progbits
