.686
.model flat, c
printf proto c: vararg
Extern K : DWORD

.data
msg db 'Output from asm module is:   %d', 0
.code
calc PROC
push ebp
mov ebp, esp
mov edx, K ; ������������ �
mov eax, dword ptr [ebp+8] ; ������������ A
mov ebx, dword ptr [ebp+12]  ; ������������ C
movsx ecx, byte ptr [ebp+16] ; ������������ E

test ebx, ebx
js negative
shr ebx, 1 ; ĳ����� C/2
jmp over

negative:
neg ebx
shr ebx, 1
neg ebx 

over:
shl ecx, 3 ; �������� E*8
sub eax, edx ; ³������� A-K
add eax, ebx ; ��������� A-K+C/2
sub eax, ecx ; ³������� A-K+C/2-E*8

pop ebp
push eax
invoke printf, offset msg, eax   
pop eax
ret
calc ENDP
END