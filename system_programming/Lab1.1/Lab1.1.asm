printf proto c : vararg

.data
EXTERN K : QWORD
msg db 'Output from asm module is: %lld', 0

.code
calc PROC
	sub rsp, 20h

	mov 	rbx, K				        ; ������������ K
	movsxd	rax, ecx					; ������������ A(32bit)
	movsxd	rcx, edx					; ������������ C(32bit)
	movsx	r9, r8b						; ������������ E(8bit)

	; Checking C sign
	test	rcx, rcx					; check C sign
	js		negative					; jump if negative
	shr		rcx, 1						; C/2
	jmp		over

	negative:
	neg		rcx							
	shr		rcx, 1						; C/2
	neg		rcx							

	over:
	shl r9, 3 ; �������� E*8
	sub rax, rbx ; ³������� A-K
	add rax, rcx ; ��������� A-K+C/2
	sub rax, r9 ; ³������� A-K+C/2-E*8
	
	mov rdx, rax
	mov r12, rax
	lea rcx, msg
	call printf
	mov rax, r12
	add rsp, 20h
	ret
calc ENDP
END 
