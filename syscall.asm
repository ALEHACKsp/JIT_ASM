public asm_syscall

.code
asm_syscall proc

	mov		r10, rcx
	mov		eax, [rsp + 40]

	add		rsp, 16
	syscall
	sub		rsp, 16

	ret

asm_syscall endp

asm_syscall2 proc

	mov		r10, rcx
	mov		eax, [rsp + 40]

	add		rsp, 16
	syscall
	sub		rsp, 16

	ret

asm_syscall2 endp

end
